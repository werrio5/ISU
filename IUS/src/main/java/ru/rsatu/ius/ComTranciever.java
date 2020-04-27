/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ru.rsatu.ius;

import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
import java.util.concurrent.LinkedTransferQueue;
import java.util.logging.Level;
import java.util.logging.Logger;
import jssc.SerialPort;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;
import jssc.SerialPortException;

/**
 *
 * @author pavel
 */
public class ComTranciever {

    private static SerialPort serialPort;
    private static Thread writingThread;
    private static int commandCounter;
    private static List<Byte> curPacket;
    private static boolean packetReady = false;
    private static boolean packetStarted = false;
    
    private static Queue<byte[]> outputCommandQueue;
    private static byte[] prevCommand;
    private static boolean sendLock;
    
    
    
    /**
     *
     * @param name
     * @throws jssc.SerialPortException
     */
    public static void openPort(String name) throws SerialPortException {
        commandCounter = 0;
        curPacket = new LinkedList<>();
        outputCommandQueue = new LinkedTransferQueue<>();
        sendLock = false;
        
        serialPort = new SerialPort(name);
        serialPort.openPort();
        serialPort.setParams(SerialPort.BAUDRATE_9600, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
        serialPort.setEventsMask(SerialPort.MASK_RXCHAR);
        serialPort.addEventListener(new EventListener());

        writingThread = new WritingThread();
        writingThread.start();
    }

    /**
     *
     * @throws jssc.SerialPortException
     */
    public static void closePort() throws SerialPortException {
        if(serialPort!=null)
        if(serialPort.isOpened())
        serialPort.closePort();
    }

    /**
     *
     * @param command
     * @throws SerialPortException
     */
    public static void writeCommand(byte[] command) throws SerialPortException {
        command[2] = (byte) (commandCounter % 16 * 17); //0x??
        byte sum = 0;
        for (int i = 1; i < command.length - 2; i++) {
            sum = (byte) ((sum + command[i]) % 256);
        }
        command[command.length - 2] = sum;
        outputCommandQueue.add(command);
    }

    private static class EventListener implements SerialPortEventListener {

        @Override
        public void serialEvent(SerialPortEvent event) {
            if (event.isRXCHAR() && event.getEventValue() >= 20) {
                try {
                    try {
                        Thread.sleep(20);
                        System.out.println(event.getEventValue());
                    } catch (InterruptedException ex) {
                        Logger.getLogger(ComTranciever.class.getName()).log(Level.SEVERE, null, ex);
                    }
                    boolean ready = readUartPacket(event.getEventValue());
                    if(ready){
                        CommandParser.receiveCommand(curPacket);
                        curPacket = new LinkedList<>();
                    }                    
                } catch (SerialPortException ex) {
                    System.out.println(ex);
                }
            }
        }
    }

    private static boolean readUartPacket(int bufferSize) throws SerialPortException {

        for (int i = 0; i < bufferSize; i++) {
            //read c
            byte[] b = serialPort.readBytes(1);
            //определение начала пакета
            if (!packetStarted) {
                if (b[0] == (byte)0xbb) {
                    packetStarted = true;
                    curPacket.add(b[0]);
                }
            } //данные или конец пакета
            else {
                //добавить в пакет
                curPacket.add(b[0]);
                //предыдущий символ
                int index = curPacket.size() - 2;
                //не пустой 
                if (index >= 0) {
                    //ищем конец пакета
                    if (curPacket.get(index) == (byte)0x00   && //конец кадра
                            b[0] ==  (byte)0x81) {              //конец пакета
                        //проверка длины пакета
                        boolean sizeControl = packetSizeControl();
                        //пакет завершен
                        if (sizeControl) {
                            packetReady = true;
                            packetStarted = false;                            
                            return packetReady;
                        }
                    }
                }

            }
        }        
        return packetReady;
    }

    private static boolean packetSizeControl() {
        if (curPacket.get(0) !=  (byte)0xbb) { //packet start
            return false;
        }
        
        int index = 1;        
        boolean correct = true;
        
        //перебор пакетов
        while (index < curPacket.size()) {
            //frame start
            if (curPacket.get(index) != (byte) 0xff) {
                //конец и нет других данных
                if(curPacket.get(index) == (byte)0x81 & index == curPacket.size()-1){
                    break;
                }
                else{
                    correct = false;
                    break;
                }
                
            }
            index++;
            index = getNextFrameEndPos(index); //param == commandID

            //frame end
            if (curPacket.get(index) != (byte) 0x00) {
                correct = false;
                break;
            }
            index++;
        }
        return correct;
    }

    private static int getNextFrameEndPos(int curCommandIdIndex) {
        byte b = curPacket.get(curCommandIdIndex);
        int offset = 0;
        switch (b) {
            //command not accepted
            case (byte)0x33: //frame start, >> comId <<, counter, control, frame end
                offset = 3;
                break;
            //command accepted
            case (byte)0xcc: //frame start, >> comId <<, counter, control, frame end
                offset = 3;
                break;
            //param input
            case (byte)0x66: //frame start, >> comId <<, counter, param id, value1, ... valueN, control, frame end
                offset = 4 + getValueBytesLength(curCommandIdIndex + 2);
                break;
            //sensor data
            case (byte)0x99: //frame start, >> comId <<, sensor id, value, control, frame end
                offset = 4;
                break;
            
        }
        return curCommandIdIndex + offset;
    }

    private static int getValueBytesLength(int paramIdIndex) {
        byte c = curPacket.get(paramIdIndex);
        int length = 0;
        switch (c) {
            case (byte)0x33: //kp, float
                length = 4;
                break;
            case (byte)0x55: //ki, float
                length = 4;
                break;
            case (byte)0x99: //kd, float
                length = 4;
                break;
            case (byte)0xaa: //tt, byte
                length = 1;
                break;
            case (byte)0x66: //st, byte
                length = 1;
                break;
            case (byte)0xcc: //ar, byte
                length = 1;
                break;
        }
        return length;
    }
    
    public static void unlock(){
        sendLock = false;
    }

    private static class WritingThread extends Thread {

        @Override
        public void run() {
            while(true){
                if(!sendLock & outputCommandQueue.size()>0){
                    sendLock = true;
                    prevCommand = outputCommandQueue.poll();
                    try {
                        serialPort.writeBytes(prevCommand);
                    } catch (SerialPortException ex) {
                        Logger.getLogger(ComTranciever.class.getName()).log(Level.SEVERE, null, ex);
                    }
                }
            }
        }
        
    }
}

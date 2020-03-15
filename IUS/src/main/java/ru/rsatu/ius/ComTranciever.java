/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ru.rsatu.ius;

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
        
    /**
     * 
     * @param name 
     * @throws jssc.SerialPortException 
     */
    public static void openPort(String name) throws SerialPortException{
        serialPort = new SerialPort(name);
        serialPort.setParams(SerialPort.BAUDRATE_9600, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
        serialPort.setEventsMask(SerialPort.MASK_RXCHAR);
        serialPort.addEventListener(new EventListener());
        serialPort.openPort();
    }
    
    /**
     * 
     * @throws jssc.SerialPortException
     */
    public static void closePort() throws SerialPortException{
        serialPort.closePort();
    }
    
    /**
     * 
     * @param command
     * @throws SerialPortException 
     */
    public static void writeCommand(String command) throws SerialPortException{
        serialPort.writeString(command);
    }
    
      static class EventListener implements SerialPortEventListener {

        @Override
        public void serialEvent(SerialPortEvent event) {
            if(event.isRXCHAR() && event.getEventValue()>=1){
                try {
                    String buffer = serialPort.readString(1);
                    ComLogger.logLine(buffer);                    
                }
                catch (SerialPortException ex) {
                    System.out.println(ex);
                }
            }
        }
     }
}

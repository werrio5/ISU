/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ru.rsatu.ius;

import java.util.List;
import java.util.Queue;
import java.util.concurrent.LinkedTransferQueue;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author pavel
 */
public class CommandParser {

    private static Queue<List<Byte>> commandQueue;
    private static Thread commandParserThread;
    private static List<Byte> curCommand = null;

    public static void init() {
        commandQueue = new LinkedTransferQueue<>();
        commandParserThread = new CommandParserThread();
        commandParserThread.start();
    }

    public static void receiveCommand(List<Byte> command) {
        commandQueue.add(command);
    }

    private static class CommandParserThread extends Thread {

        @Override
        public void run() {
            while (true) {
                if (!commandQueue.isEmpty()) {
                    if(curCommand==null){
                        curCommand = commandQueue.poll();
                        parse();
                        curCommand=null;
                    }                    
                }

            }
        }

        private void parse() {
            //
            Integer curTemp = null;
            Integer curFan = null;
            //0 -packet start, 1 frame start
            int curFrameComIDIndex = 2;
            while (curFrameComIDIndex < curCommand.size()) {
                byte curFrameComID = curCommand.get(curFrameComIDIndex);
                switch (curFrameComID) {
                    //command not accepted
                    case (byte) 0x33:
                        ComLogger.logLine("not accepted");
                        break;
                    //command accepted
                    case (byte) 0xcc:
                        ComLogger.logLine("accepted");
                        break;
                    //param input
                    case (byte) 0x66:
                        ComLogger.logLine("param input");
                        break;
                    //sensor data
                    case (byte) 0x99:
                        ComLogger.logLine("sensor data");
                        byte curFrameSensorID = curCommand.get(curFrameComIDIndex+1);
                        
                        switch(curFrameSensorID){
                            case (byte) 0xfa: //temp
                            {
                                curTemp = (int)curCommand.get(curFrameComIDIndex+2);
                                ComLogger.logLine("temp = "+curTemp);
                                break;
                            }
                            case (byte) 0xaf: //fan
                            {
                                curFan = (int)curCommand.get(curFrameComIDIndex+2);
                                ComLogger.logLine("fan = "+curFan);
                                break;
                            }
                        }
                        break;
                }
                curFrameComIDIndex = getNextFrameEndPos(curFrameComIDIndex)+2;
            }
            Chart.addData(curTemp, curFan);
        }

        private static int getNextFrameEndPos(int curCommandIdIndex) {
            byte b = curCommand.get(curCommandIdIndex);
            int offset = 0;
            switch (b) {
                //command not accepted
                case (byte) 0x33: //frame start, >> comId <<, counter, control, frame end
                    offset = 3;
                    break;
                //command accepted
                case (byte) 0xcc: //frame start, >> comId <<, counter, control, frame end
                    offset = 3;
                    break;
                //param input
                case (byte) 0x66: //frame start, >> comId <<, counter, param id, value1, ... valueN, control, frame end
                    offset = 4 + getValueBytesLength(curCommandIdIndex + 2);
                    break;
                //sensor data
                case (byte) 0x99: //frame start, >> comId <<, sensor id, value, control, frame end
                    offset = 4;
                    break;

            }
            return curCommandIdIndex + offset;
        }

        private static int getValueBytesLength(int paramIdIndex) {
            byte c = curCommand.get(paramIdIndex);
            int length = 0;
            switch (c) {
                case (byte) 0x33: //kp, float
                    length = 4;
                    break;
                case (byte) 0x55: //ki, float
                    length = 4;
                    break;
                case (byte) 0x99: //kd, float
                    length = 4;
                    break;
                case (byte) 0xaa: //tt, byte
                    length = 1;
                    break;
                case (byte) 0x66: //st, byte
                    length = 1;
                    break;
                case (byte) 0xcc: //ar, byte
                    length = 1;
                    break;
            }
            return length;
        }

    }
}

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ru.rsatu.ius;

import java.util.Queue;
import java.util.concurrent.LinkedTransferQueue;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author pavel
 */
public class CommandParser {
    
    private static Queue<String> commandQueue;
    private static Thread commandParserThread;
    
    public static void init(){
        commandQueue = new LinkedTransferQueue<>();
        commandParserThread = new CommandParserThread();
        commandParserThread.start();
    }
    
    public static void receiveCommand(String command){
        
        commandQueue.add(command);
    }
    
    private static class CommandParserThread extends Thread {
        
        @Override
        public void run() {
            while(true)
            {
               if(!commandQueue.isEmpty()){
                   String s = commandQueue.poll();
                   System.out.println(s);
                   ComLogger.logLine(s);
               }
                
            }            
        }
        
    }
}

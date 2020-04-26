/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ru.rsatu.ius;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JFileChooser;
import javax.swing.JTextArea;

/**
 *
 * @author pavel
 */
public class ComLogger {
    
    private static final DateTimeFormatter dtf = DateTimeFormatter.ofPattern("yyyy/MM/dd HH:mm:ss"); 
    private static JTextArea logBox;
    private static StringBuilder logToSave;
    
    /**
     * указать место вывода
     * @param lb
     */
    public static void init(JTextArea lb){
        logBox = lb;
        logToSave = new StringBuilder();
    }
    
    /**
     * вывод строки в textarea
     * @param text 
     */
    public static void logLine(String text){
        StringBuilder message = new StringBuilder();
        message.append(dtf.format(LocalDateTime.now())).append(" : ");
        message.append(text);
        logToSave.append("\n"+message.toString());
        logBox.append("\n"+message.toString());
        logBox.setCaretPosition(logBox.getDocument().getLength());
    }  
    
    public static void saveLogToFile(){
        try {
            String s = new Date().toString().replaceAll(":", "-");
            FileOutputStream FOS = new FileOutputStream(s+".log");
            FOS.write(logToSave.toString().getBytes());
            FOS.close();
        } catch (IOException ex) {
            Logger.getLogger(ComLogger.class.getName()).log(Level.SEVERE, null, ex);
        }
        
    }
    
}

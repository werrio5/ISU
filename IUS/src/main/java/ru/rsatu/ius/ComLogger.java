/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ru.rsatu.ius;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Date;
import java.util.logging.Logger;
import javax.swing.JTextArea;

/**
 *
 * @author pavel
 */
public class ComLogger {
    
    private static final DateTimeFormatter dtf = DateTimeFormatter.ofPattern("yyyy/MM/dd HH:mm:ss"); 
    private static JTextArea logBox;
    
    /**
     * указать место вывода
     * @param lb
     */
    public static void init(JTextArea lb){
        logBox = lb;
    }
    
    /**
     * вывод строки в textarea
     * @param text 
     */
    public static void logLine(String text){
        StringBuilder message = new StringBuilder();
        message.append(dtf.format(LocalDateTime.now())).append(" : ");
        message.append(text);
        logBox.append("\n"+message.toString());
    }  
    
}

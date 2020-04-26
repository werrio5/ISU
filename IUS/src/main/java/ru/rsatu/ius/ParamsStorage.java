/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ru.rsatu.ius;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author pavel
 */
public class ParamsStorage {
    
    private static float kp;
    private static float ki;
    private static float kd;
    private static byte active_relay;
    private static byte shutdown_temp;
    private static byte target_temp;

    public static float getKp() {
        return kp;
    }

    public static void setKp(float kp) {
        ParamsStorage.kp = kp;
        try {
            saveToFile();
        } catch (IOException ex) {
            Logger.getLogger(ParamsStorage.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public static float getKi() {
        return ki;
    }

    public static void setKi(float ki) {
        ParamsStorage.ki = ki;
        try {
            saveToFile();
        } catch (IOException ex) {
            Logger.getLogger(ParamsStorage.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public static float getKd() {
        return kd;
    }

    public static void setKd(float kd) {
        ParamsStorage.kd = kd;
        try {
            saveToFile();
        } catch (IOException ex) {
            Logger.getLogger(ParamsStorage.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public static byte getActive_relay() {
        return active_relay;
    }

    public static void setActive_relay(byte active_relay) {
        ParamsStorage.active_relay = active_relay;
        try {
            saveToFile();
        } catch (IOException ex) {
            Logger.getLogger(ParamsStorage.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public static byte getShutdown_temp() {
        return shutdown_temp;
    }

    public static void setShutdown_temp(byte shutdown_temp) {
        ParamsStorage.shutdown_temp = shutdown_temp;
        try {
            saveToFile();
        } catch (IOException ex) {
            Logger.getLogger(ParamsStorage.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public static byte getTarget_temp() {
        return target_temp;
    }

    public static void setTarget_temp(byte target_temp) {
        ParamsStorage.target_temp = target_temp;
        try {
            saveToFile();
        } catch (IOException ex) {
            Logger.getLogger(ParamsStorage.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    
    
    public static void init(){
        try {
            if(!new File("parameters").exists())
                saveToFile();            
            readFromFile();    
        } catch (IOException ex) {
            Logger.getLogger(ParamsStorage.class.getName()).log(Level.SEVERE, null, ex);
            
        }
    }
        
    private static void saveToFile() throws IOException{
        FileWriter fileWriter = new FileWriter("parameters");
        PrintWriter printWriter = new PrintWriter(fileWriter);
        printWriter.println(kp);
        printWriter.println(ki);
        printWriter.println(kd);
        printWriter.println(active_relay);
        printWriter.println(shutdown_temp);
        printWriter.println(target_temp);
        printWriter.close();
    }
    
    private static void readFromFile() throws IOException{
        BufferedReader br = new BufferedReader(new FileReader("parameters"));   
        String st = br.readLine(); 
        kp = Float.valueOf(st);
        st = br.readLine(); 
        ki = Float.valueOf(st);
        st = br.readLine(); 
        kd = Float.valueOf(st);
        st = br.readLine(); 
        active_relay = Byte.valueOf(st);
        st = br.readLine(); 
        shutdown_temp = Byte.valueOf(st);
        st = br.readLine(); 
        target_temp = Byte.valueOf(st);
    }
}

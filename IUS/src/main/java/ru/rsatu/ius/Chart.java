/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ru.rsatu.ius;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.geom.Point2D;
import java.util.LinkedList;
import java.util.List;
import javax.swing.JFrame;
import javax.swing.JPanel;
import jdk.nashorn.internal.objects.Global;

/**
 *
 * @author pavel
 */
public class Chart extends JFrame{
    
    private static JPanel drawingField;
    private static List<Integer> tempList;
    private static List<Integer> fanList;
    
    static int valuesOnScreen = 500;
    
    public Chart(JPanel panel) {
        drawingField = panel;
        tempList = new LinkedList<>();
        fanList = new LinkedList<>();
    }
    
    private static void drawData(List<Integer> data, Color color){
        int firstElementIndex = data.size() > valuesOnScreen ? data.size() - valuesOnScreen -1 : 0;
        int visibleValuesCount = data.size() > valuesOnScreen ? valuesOnScreen : data.size();
        //y boundaries
        int max = 0;
        int min = 9999;        
        
        for(int i=firstElementIndex; i<data.size(); i++){
            if(data.get(i) > max) max = data.get(i);
            if(data.get(i) < min) min = data.get(i);
        }
        int height = drawingField.getHeight();
        double scale = max-min > 0 ? (double)height/(double)(max-min) : 1;
        
        Graphics g = drawingField.getGraphics();
        g.setColor(color);
        
        Point2D.Double prev = new Point2D.Double(0, data.get(firstElementIndex) * scale);
        for(int i = 1; i<visibleValuesCount; i++){
           Point2D.Double cur = new Point2D.Double(i, data.get(firstElementIndex+i) * scale); 
           drawLine(g,prev,cur);
           prev = cur;
        }
    }
    
    private static void drawLine(Graphics g, Point2D.Double prev, Point2D.Double cur){
        g.drawLine((int)(prev.x), (int)(-prev.y + drawingField.getHeight()), (int)(cur.x), (int)(-cur.y + drawingField.getHeight()));
    }
    
    public static void addData(Integer temp, Integer fan){
        if(temp==null) tempList.add(0);
        else tempList.add(temp);
        
        if(fan==null) fanList.add(0);
        else fanList.add(fan);
        
        Graphics g = drawingField.getGraphics();
        g.clearRect(0, 0, drawingField.getWidth(), drawingField.getHeight());
        drawData(tempList, Color.red);
        drawData(fanList, Color.blue);
    }
}
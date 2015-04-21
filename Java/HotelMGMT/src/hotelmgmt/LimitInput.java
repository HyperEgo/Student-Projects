/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package hotelmgmt;

import javax.swing.text.PlainDocument;
import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;

/**
 *
 * @author fc
 */
public class LimitInput extends PlainDocument{
    
  private int limit;

  LimitInput(int limit) {
   super();
   this.limit = limit;
   }
 
  public void insertString( int offset, String  str, AttributeSet attr ) throws BadLocationException {
    if (str == null) return;
    if ((getLength() + str.length()) <= limit) {
      super.insertString(offset, str, attr);
    }
  }    
    
}

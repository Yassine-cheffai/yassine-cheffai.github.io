// C4D-DialogResource
DIALOG IDC_SERIALSERVERMANAGER
{
  NAME IDS_DIALOG; SCALE_V; SCALE_H; 
  
  GROUP IDC_STATIC
  {
    NAME IDS_STATIC3; SCALE_V; SCALE_H; 
    BORDERSIZE 4, 4, 4, 4; 
    COLUMNS 1;
    ALLOW_WEIGHTS; 
    SPACE 4, 4;
    
    GROUP IDC_STATIC
    {
      NAME IDS_STATIC6; ALIGN_TOP; ALIGN_LEFT; 
      BORDERSIZE 0, 0, 0, 0; 
      ROWS 1;
      EQUAL_COLS; 
      SPACE 4, 4;
      
      
    }
    GROUP IDC_STATIC
    {
      NAME IDS_STATIC8; SCALE_V; SCALE_H; 
      BORDERSIZE 0, 0, 0, 0; 
      ROWS 2;
      ALLOW_WEIGHTS; 
      SPACE 4, 4;
      
      GROUP IDC_STATIC
      {
        NAME IDS_STATIC10; ALIGN_TOP; SCALE_H; 
        BORDERSIZE 0, 0, 0, 0; 
        ROWS 1;
        SPACE 4, 4;
        
        STATICTEXT IDC_STATIC { NAME IDS_STATIC1; CENTER_V; ALIGN_LEFT; }
        STATICTEXT IDC_STATIC { NAME IDS_STATIC11; CENTER_V; SCALE_H; }
        BUTTON IDC_ADDSERIALS { NAME IDS_BUTTON1; ALIGN_TOP; ALIGN_LEFT; }
        BUTTON IDC_SETSERVERINFO { NAME IDS_BUTTON3; ALIGN_TOP; ALIGN_LEFT; }
      }
      TREEVIEW IDC_SERIALLIST1
      {
        SCALE_V; SCALE_H; 
        BORDER; 
        HAS_HEADER; 
        RESIZE_HEADER; 
        CURSORKEYS; 
      }
      GROUP IDC_STATIC
      {
        NAME IDS_STATIC12; ALIGN_TOP; SCALE_H; 
        BORDERSIZE 0, 0, 0, 0; 
        ROWS 1;
        SPACE 4, 4;
        
        STATICTEXT IDC_STATIC { NAME IDS_STATIC9; CENTER_V; ALIGN_LEFT; }
        STATICTEXT IDC_STATIC { NAME IDS_STATIC13; CENTER_V; SCALE_H; }
        BUTTON IDC_ADDGROUP { NAME IDS_BUTTON2; ALIGN_TOP; ALIGN_LEFT; }
      }
      TREEVIEW IDC_GROUPLIST
      {
        SCALE_V; SCALE_H; 
        BORDER; 
        HIDE_LINES; 
        HAS_HEADER; 
        RESIZE_HEADER; 
        CURSORKEYS; 
      }
      GROUP IDC_STATIC
      {
        NAME IDS_STATIC14; ALIGN_TOP; SCALE_H; 
        BORDERSIZE 0, 0, 0, 0; 
        ROWS 1;
        SPACE 4, 4;
        
        STATICTEXT IDC_STATIC { NAME IDS_STATIC2; CENTER_V; ALIGN_LEFT; }
        STATICTEXT IDC_STATIC { NAME IDS_STATIC15; CENTER_V; SCALE_H; }
        STATICTEXT IDC_STATIC { NAME IDS_STATIC16; CENTER_V; ALIGN_LEFT; }
        COMBOBOX IDC_GROUPBY
        {
          ALIGN_TOP; ALIGN_LEFT; SIZE 150, 0; 
          CHILDS
          {
            0, IDS_GROUP1; 
            1, IDS_GROUP2; 
            2, IDS_GROUP3; 
            3, IDS_GROUP4; 
            4, IDS_GROUP5; 
            5, IDS_GROUP6; 
            6, IDS_GROUP7; 
          }
        }
      }
      TREEVIEW IDC_SERIALLIST2
      {
        SCALE_V; SCALE_H; 
        BORDER; 
        HAS_HEADER; 
        RESIZE_HEADER; 
        CURSORKEYS; 
      }
    }
    GROUP IDC_STATIC
    {
      SCALE_V; SCALE_H; 
      BORDERSIZE 0, 0, 0, 0; 
      COLUMNS 1;
      ALLOW_WEIGHTS; 
      SPACE 4, 4;
      
      STATICTEXT IDC_STATIC { NAME IDS_STATIC5; CENTER_V; ALIGN_LEFT; }
      GROUP IDC_STATIC
      {
        SCALE_V; SCALE_H; 
        BORDERSTYLE BORDER_THIN_IN; BORDERSIZE 2, 2, 2, 2; 
        COLUMNS 1;
        SPACE 4, 4;
        
        DESCRIPTION IDC_DESCRIPTION
        {
          SCALE_V; SCALE_H; 
          ALLOWFOLDING; 
          SHOWTITLE; 
          OBJECTSNOTINDOC; 
        }
      }
    }
  }
}
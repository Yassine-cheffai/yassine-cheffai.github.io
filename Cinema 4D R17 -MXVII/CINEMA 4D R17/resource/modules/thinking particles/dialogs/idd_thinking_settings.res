// C4D-DialogResource
DIALOG IDD_THINKING_SETTINGS
{
  NAME TDLG; SCALE_V; SCALE_H; 
  
  GROUP 
  {
    SCALE_V; SCALE_H; 
    BORDERSIZE 0, 0, 0, 0; 
    COLUMNS 1;
    SPACE 4, 4;
    
    TAB 
    {
      SCALE_V; SCALE_H; 
      SELECTION_TABS; 
      
      GROUP IDC_TAB1
      {
        NAME TB1; SCALE_V; SCALE_H; 
        BORDERSIZE 4, 4, 4, 4; 
        COLUMNS 1;
        SPACE 4, 4;
        
        GROUP 
        {
          ALIGN_TOP; SCALE_H; 
          BORDERSIZE 0, 0, 0, 0; 
          ROWS 1;
          SPACE 4, 4;
          
          GROUP IDC_STATIC
          {
            NAME IDS_STATIC1; ALIGN_TOP; SCALE_H; 
            BORDERSIZE 0, 0, 0, 0; 
            COLUMNS 5;
            EQUAL_ROWS; 
            SPACE 4, 4;
            
            STATICTEXT  { NAME NUMLIMIT; CENTER_V; FIT_H; }
            EDITNUMBERARROWS IDC_TP_NUMLIMIT
            { SCALE_V; FIT_H; }
            STATICTEXT IDC_STATIC { NAME IDS_STATIC2; CENTER_V; ALIGN_LEFT; SIZE 20, 0; }
            STATICTEXT  { NAME TOTAL; CENTER_V; FIT_H; }
            STATICTEXT IDC_TP_CTOTAL { CENTER_V; SCALE_H; }
            STATICTEXT  { NAME FORCETHIS; CENTER_V; FIT_H; }
            CHECKBOX IDC_TP_FORCETHIS { SCALE_V; FIT_H;  }
            STATICTEXT IDC_STATIC { NAME IDS_STATIC3; CENTER_V; ALIGN_LEFT; SIZE 20, 0; }
            STATICTEXT  { NAME TREE; CENTER_V; FIT_H; }
            STATICTEXT IDC_TP_CTREE { CENTER_V; SCALE_H; }
            STATICTEXT  { NAME SHOWOBJECT; CENTER_V; FIT_H; }
            CHECKBOX IDC_TP_SHOWOBJECT { SCALE_V; FIT_H;  }
            STATICTEXT IDC_STATIC { NAME IDS_STATIC4; CENTER_V; ALIGN_LEFT; SIZE 20, 0; }
            STATICTEXT  { NAME GROUP; CENTER_V; FIT_H; }
            STATICTEXT IDC_TP_CGROUP { CENTER_V; SCALE_H; }
            STATICTEXT  { NAME VIEWTYPE; CENTER_V; FIT_H; }
            COMBOBOX IDC_TP_VIEWTYPE
            {
              SCALE_V; FIT_H; 
              CHILDS
              {
                0, NONE; 
                1, FLAKES; 
                2, DOTS; 
                3, TICKS; 
                4, DROPS; 
                5, BOX; 
              }
            }
          }
        }
        SEPARATOR { SCALE_H; }
        GROUP 
        {
          SCALE_V; SCALE_H; 
          BORDERSIZE 0, 0, 0, 0; 
          COLUMNS 2;
          SPACE 4, 4;
          
          STATICTEXT  { NAME PARTICLEGROUPS; CENTER_V; FIT_H; }
          STATICTEXT  { NAME PARTICLEGROUPS2; CENTER_V; FIT_H; }
          TREEVIEW IDC_TP_GROUP
          {
            SCALE_V; SCALE_H;
            BORDER; 
          }
          DESCRIPTION IDC_TP_GROUPSETTINGS
          {
            SCALE_V; SCALE_H;
            ALLOWFOLDING; 
          }
        }
      }
      GROUP IDC_TAB2
      {
        NAME TB2; SCALE_V; SCALE_H; 
        BORDERSIZE 4, 4, 4, 4; 
        COLUMNS 1;
        SPACE 4, 4;
        
        GROUP 
        {
          SCALE_V; SCALE_H; 
          BORDERSIZE 0, 0, 0, 0; 
          COLUMNS 1;
          SPACE 4, 4;
          
          STATICTEXT  { NAME DATACHANNELS; CENTER_V; FIT_H; }
          LISTVIEW IDC_TP_CHAN { SCALE_V; SCALE_H; }
          EDITTEXT IDC_TP_DATANAME
          { CENTER_V; FIT_H; }
          COMBOBOX IDC_TP_DATAS
          {
            CENTER_V; FIT_H; SIZE -30, 0; 
            CHILDS
            {
            }
          }
        }
        GROUP 
        {
          ALIGN_TOP; CENTER_H; 
          BORDERSIZE 0, 0, 0, 0; 
          COLUMNS 2;
          SPACE 4, 4;
          
          BUTTON IDC_TP_ADDDATA { NAME ADD; CENTER_V; FIT_H; }
          BUTTON IDC_TP_REMOVEDATA { NAME REMOVE; CENTER_V; FIT_H; }
        }
      }
    }
  }
}
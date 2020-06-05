// C4D-DialogResource
DIALOG IDD_UPDATE_DIALOG
{
  NAME IDS_UPDATE_DIALOG; SCALE_V; SCALE_H; 
  
  GROUP IDC_STATIC
  {
    NAME IDS_STATIC32; SCALE_V; SCALE_H; 
    BORDERSIZE 4, 4, 4, 4; 
    COLUMNS 1;
    SPACE 4, 4;
    
    GROUP IDC_STATIC
    {
      NAME IDS_STATIC44; SCALE_V; SCALE_H; 
      BORDERSIZE 0, 0, 0, 0; 
      COLUMNS 2;
      SPACE 8, 4;
      
      GROUP IDC_STATIC
      {
        SCALE_V; ALIGN_LEFT; 
        BORDERSTYLE BORDER_THIN_IN; BORDERSIZE 0, 0, 0, 0; 
        COLUMNS 1;
        SPACE 4, 4;
        
        USERAREA IDC_UPDATE_LOGO_USER_AREA { SCALE_V; SCALE_H; SIZE 120, 100; }
      }
      TAB IDC_UPDATE_GROUP
      {
        SCALE_V; SCALE_H; 
        SELECTION_NONE; 
        
        GROUP IDC_UPDATE_CHECK_GROUP
        {
          NAME IDS_STATIC46; SCALE_V; SCALE_H; 
          BORDERSIZE 0, 0, 0, 0; 
          COLUMNS 1;
          SPACE 4, 4;
          
          USERAREA IDC_UPDATE_CHECK_AREA { SCALE_V; SCALE_H; SIZE 120, 100; }
        }
        GROUP IDC_INSTALL_README_GROUP
        {
          NAME IDS_STATIC95; SCALE_V; SCALE_H; 
          BORDERSIZE 0, 0, 0, 0; 
          COLUMNS 1;
          SPACE 4, 4;
          
          HTMLVIEWER IDC_INSTALL_README_HTML
          {
            SCALE_V; SCALE_H; 
          }
        }
        GROUP IDC_INSTALL_SERIALS_GROUP
        {
          NAME IDS_STATIC71; SCALE_V; SCALE_H; 
          BORDERSIZE 0, 0, 0, 0; 
          ROWS 1;
          SPACE 4, 4;
          
          GROUP IDC_STATIC
          {
            NAME IDS_STATIC72; SCALE_V; SCALE_H; 
            BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 4, 4, 4, 4; 
            COLUMNS 1;
            SPACE 4, 4;
            
            GROUP IDC_STATIC
            {
              NAME IDS_STATIC76; ALIGN_TOP; SCALE_H; 
              BORDERSIZE 0, 0, 0, 8; 
              COLUMNS 2;
              SPACE 4, 4;
              
              STATICTEXT IDC_STATIC { NAME IDS_STATIC77; CENTER_V; ALIGN_LEFT; }
              EDITTEXT IDC_INSTALL_NAME_EDIT
              { CENTER_V; SCALE_H; SIZE 70, 0; }
              STATICTEXT IDC_STATIC { NAME IDS_STATIC78; CENTER_V; ALIGN_LEFT; }
              EDITTEXT IDC_INSTALL_COMPANY_EDIT
              { CENTER_V; SCALE_H; SIZE 70, 0; }
              STATICTEXT IDC_STATIC { NAME IDS_STATIC79; CENTER_V; ALIGN_LEFT; }
              EDITTEXT IDC_INSTALL_STREET_EDIT
              { CENTER_V; SCALE_H; SIZE 70, 0; }
              STATICTEXT IDC_STATIC { NAME IDS_STATIC80; CENTER_V; ALIGN_LEFT; }
              EDITTEXT IDC_INSTALL_CITY_EDIT
              { CENTER_V; SCALE_H; SIZE 70, 0; }
              STATICTEXT IDC_STATIC { NAME IDS_STATIC81; CENTER_V; ALIGN_LEFT; }
              EDITTEXT IDC_INSTALL_COUNTRY_EDIT
              { CENTER_V; SCALE_H; SIZE 70, 0; }
            }
            SEPARATOR { SCALE_H; }
            GROUP IDC_STATIC
            {
              NAME IDS_STATIC99; ALIGN_TOP; SCALE_H; 
              BORDERSIZE 0, 8, 0, 0; 
              COLUMNS 2;
              SPACE 4, 4;
              
              STATICTEXT IDC_STATIC { NAME IDS_STATIC100; CENTER_V; SCALE_H; }
              BUTTON IDC_INSTALL_SERIALS_PASTE_BTN { NAME IDS_INSTALL_SERIALS_PASTE; ALIGN_TOP; ALIGN_LEFT; }
            }
            MULTILINEEDIT IDC_INSTALL_SERIALS_EDIT
            { SCALE_V; SCALE_H; SIZE 70, 0; }
          }
          GROUP IDC_STATIC
          {
            NAME IDS_STATIC73; SCALE_V; SCALE_H; 
            BORDERSTYLE BORDER_GROUP_IN; BORDERSIZE 4, 4, 4, 4; 
            COLUMNS 1;
            SPACE 4, 4;
            
            GROUP IDC_STATIC
            {
              NAME IDS_STATIC105; SCALE_V; SCALE_H; 
              BORDERSIZE 4, 4, 4, 4; 
              COLUMNS 1;
              SPACE 4, 4;
              
              GROUP IDC_INSTALL_SERIALS_INFO_GROUP
              {
                NAME IDS_STATIC74; SCALE_V; SCALE_H; 
                BORDERSIZE 0, 0, 0, 0; 
                COLUMNS 2;
                SPACE 4, 4;
                
                
              }
            }
            STATICTEXT IDC_STATIC { NAME IDS_STATIC107; SCALE_V; ALIGN_LEFT; }
            GROUP IDC_STATIC
            {
              NAME IDS_STATIC123; ALIGN_TOP; SCALE_H; 
              BORDERSIZE 0, 0, 0, 0; 
              ROWS 1;
              SPACE 4, 4;
              
              STATICTEXT IDC_STATIC { NAME IDS_STATIC122; CENTER_V; ALIGN_LEFT; }
              COMBOBOX IDC_INSTALL_SERIALS_MODE_COMBO
              {
                ALIGN_TOP; SCALE_H; SIZE 150, 0; 
                CHILDS
                {
                  0, IDS_INSTALL_SERIALS_MODE_CURRENT_USER; 
                  1, IDS_INSTALL_SERIALS_MODE_GLOBAL; 
                }
              }
            }
            CHECKBOX IDC_INSTALL_SERIALS_LICENSE_SERVER_CHK { NAME IDS_INSTALL_SERIALS_USE_LIC_SERVER; ALIGN_BOTTOM; ALIGN_LEFT;  }
          }
        }
        GROUP IDC_INSTALL_TYPE_GROUP
        {
          NAME IDS_STATIC101; SCALE_V; SCALE_H; 
          BORDERSIZE 0, 0, 0, 0; 
          COLUMNS 1;
          SPACE 4, 4;
          
          GROUP IDC_STATIC
          {
            NAME IDS_STATIC104; CENTER_V; ALIGN_LEFT; 
            BORDERSIZE 0, 0, 0, 0; 
            COLUMNS 1;
            SPACE 4, 4;
            
            STATICTEXT IDC_STATIC { NAME IDS_STATIC102; CENTER_V; ALIGN_LEFT; }
            STATICTEXT IDC_STATIC { NAME IDS_STATIC106; CENTER_V; ALIGN_LEFT; }
            RADIOGROUP IDC_STATIC
            {
              NAME IDS_STATIC103; ALIGN_TOP; ALIGN_LEFT; 
              BORDERSIZE 0, 0, 0, 0; 
              COLUMNS 1;
              SPACE 4, 4;
              
              RADIOGADGET IDC_INSTALL_TYPE_C4D_BP_RBT { NAME IDS_INSTALL_TYPE_C4D_BP; ALIGN_TOP; ALIGN_LEFT;  }
              RADIOGADGET IDC_INSTALL_TYPE_BP_RBT { NAME IDS_INSTALL_TYPE_BP; ALIGN_TOP; ALIGN_LEFT;  }
              RADIOGADGET IDC_INSTALL_TYPE_NET_SERVER_RBT { NAME IDS_INSTALL_TYPE_NET_SERVER; ALIGN_TOP; ALIGN_LEFT;  }
              RADIOGADGET IDC_INSTALL_TYPE_NET_CLIENT_RBT { NAME IDS_INSTALL_TYPE_NET_CLIENT; ALIGN_TOP; ALIGN_LEFT;  }
              RADIOGADGET IDC_INSTALL_TYPE_LICSERVER_RBT { NAME IDS_INSTALL_TYPE_LICENSE_SERVER; ALIGN_TOP; ALIGN_LEFT;  }
              RADIOGADGET IDC_INSTALL_TYPE_CINEBENCH_RBT { NAME IDS_INSTALL_TYPE_CINEBENCH; ALIGN_TOP; ALIGN_LEFT;  }
              RADIOGADGET IDC_INSTALL_TYPE_MODULES_RBT { NAME IDS_INSTALL_TYPE_MODULES; ALIGN_TOP; ALIGN_LEFT;  }
            }
          }
        }
        GROUP IDC_UPDATE_NOTIFY_GROUP
        {
          NAME IDS_UPDATE_NOTIFICATION; SCALE_V; SCALE_H; 
          BORDERSIZE 0, 0, 0, 0; 
          COLUMNS 1;
          ALLOW_WEIGHTS; 
          SPACE 4, 4;
          
          GROUP IDC_STATIC
          {
            NAME IDS_STATIC57; SCALE_V; SCALE_H; 
            BORDERSIZE 0, 0, 0, 0; 
            COLUMNS 1;
            SPACE 0, 4;
            
            QUICKTAB IDC_UPDATE_NOTIFY_TAB
            {
              ALIGN_TOP; SCALE_H; 
              SHOWSINGLE; 
              NOMULTISELECT; 
            }
            TREEVIEW IDC_UPDATE_NOTIFY_TREE
            {
              SCALE_V; SCALE_H; 
              BORDER; 
              HIDE_LINES; 
              CTRL_DRAG; 
              HAS_HEADER; 
              FIXED_LAYOUT; 
              ALTERNATE_BG; 
              CURSORKEYS; 
            }
          }
          GROUP IDC_STATIC
          {
            NAME IDS_STATIC4; SCALE_V; SCALE_H; 
            BORDERSIZE 0, 0, 0, 0; 
            COLUMNS 1;
            ALLOW_WEIGHTS; 
            SPACE 4, 4;
            
            STATICTEXT IDC_STATIC { NAME IDS_UPDATE_DESCRIPTION; CENTER_V; ALIGN_LEFT; }
            HTMLVIEWER IDC_UPDATE_NOTIFY_DESCRIPTION_HTML
            {
              SCALE_V; SCALE_H; 
            }
          }
        }
        GROUP IDC_UPDATE_SETTINGS_GROUP
        {
          NAME IDS_UPDATE_SETTINGS; CENTER_V; SCALE_H; 
          BORDERSIZE 4, 4, 4, 4; 
          COLUMNS 1;
          SPACE 4, 4;
          
          CHECKBOX IDC_UPDATE_SETTINGS_AUTO_RESTART_CHK { NAME IDS_UPDATE_SETTINGS_AUTO_RESTART; ALIGN_TOP; ALIGN_LEFT;  }
          CHECKBOX IDC_UPDATE_SETTINGS_REMOVE_FILES_CHK { NAME IDS_UPDATE_SETTINGS_REMOVE_FILES; ALIGN_TOP; ALIGN_LEFT;  }
          CHECKBOX IDC_UPDATE_SETTINGS_CREATE_BACKUP_CHK { NAME IDS_CREATE_BACKUP_COPY; ALIGN_TOP; ALIGN_LEFT;  }
          GROUP IDC_STATIC
          {
            NAME IDS_STATIC52; ALIGN_TOP; SCALE_H; 
            BORDERSIZE 0, 0, 0, 0; 
            COLUMNS 2;
            SPACE 4, 4;
            
            STATICTEXT IDC_STATIC { NAME IDS_STATIC53; CENTER_V; ALIGN_LEFT; }
            FILENAME IDC_UPDATE_SETTINGS_CREATE_BACKUP_PATH
            {
              ALIGN_TOP; SCALE_H; 
              DIRECTORY; 
            }
          }
          STATICTEXT IDC_STATIC { NAME IDS_STATIC58; CENTER_V; ALIGN_LEFT; }
        }
        GROUP IDC_INSTALL_SETTINGS_GROUP
        {
          NAME IDS_STATIC61; CENTER_V; SCALE_H; 
          BORDERSIZE 0, 0, 0, 0; 
          COLUMNS 1;
          SPACE 4, 4;
          
          GROUP IDC_STATIC
          {
            NAME IDS_STATIC69; ALIGN_TOP; SCALE_H; 
            BORDERSIZE 0, 0, 0, 20; 
            COLUMNS 1;
            SPACE 4, 4;
            
            STATICTEXT IDC_INSTALL_SETTINGS_PATH_STATIC { NAME IDS_STATIC62; CENTER_V; SCALE_H; }
            FILENAME IDC_INSTALL_SETTINGS_PATH
            {
              ALIGN_TOP; SCALE_H; 
              DIRECTORY; 
            }
            GROUP IDC_STATIC
            {
              NAME IDS_STATIC89; ALIGN_TOP; SCALE_H; 
              BORDERSIZE 0, 0, 0, 0; 
              COLUMNS 2;
              SPACE 4, 4;
              
              STATICTEXT IDC_STATIC { NAME IDS_STATIC90; CENTER_V; ALIGN_LEFT; }
              STATICTEXT IDC_INSTALL_SETTINGS_SPACE_REQUIRED_STATIC { NAME IDS_STATIC92; CENTER_V; SCALE_H; }
              STATICTEXT IDC_STATIC { NAME IDS_STATIC91; CENTER_V; ALIGN_LEFT; }
              STATICTEXT IDC_INSTALL_SETTINGS_SPACE_AVAILABLE_STATIC { NAME IDS_STATIC93; CENTER_V; SCALE_H; }
            }
          }
          GROUP IDC_INSTALL_SETTINGS_WINDOWS_GROUP
          {
            NAME IDS_STATIC67; ALIGN_TOP; SCALE_H; 
            BORDERSIZE 0, 0, 0, 20; 
            COLUMNS 1;
            SPACE 4, 4;
            
            STATICTEXT IDC_STATIC { NAME IDS_STATIC68; CENTER_V; SCALE_H; }
            RADIOGADGET IDC_INSTALL_SETTINGS_WIN_NO_ENTRY_RBT { NAME IDS_INSTALL_SETTINGS_WIN_NO_ENTRY; ALIGN_TOP; ALIGN_LEFT;  }
            RADIOGADGET IDC_INSTALL_SETTINGS_WIN_ONLY_ME_RBT { NAME IDS_INSTALL_SETTINGS_WIN_ONLY_ME; ALIGN_TOP; ALIGN_LEFT;  }
            RADIOGADGET IDC_INSTALL_SETTINGS_WIN_EVERYONE_RBT { NAME IDS_INSTALL_SETTINGS_WIN_EVERYONE; ALIGN_TOP; ALIGN_LEFT;  }
          }
          GROUP IDC_INSTALL_SETTINGS_WINDOWS_1_GROUP
          {
            NAME IDS_STATIC70; ALIGN_TOP; SCALE_H; 
            BORDERSIZE 0, 0, 0, 0; 
            COLUMNS 1;
            SPACE 4, 4;
            
            CHECKBOX IDC_INSTALL_SETTINGS_WIN_DESKTOP_CHK { NAME IDS_INSTALL_SETTINGS_WIN_DESKTOP; ALIGN_TOP; SCALE_H;  }
          }
        }
        GROUP IDC_UPDATE_LICENSE_GROUP
        {
          NAME IDS_STATIC43; SCALE_V; SCALE_H; 
          BORDERSIZE 0, 0, 0, 0; 
          COLUMNS 1;
          SPACE 4, 4;
          
          GROUP IDC_STATIC
          {
            NAME IDS_STATIC60; ALIGN_TOP; SCALE_H; 
            BORDERSIZE 0, 0, 0, 0; 
            ROWS 1;
            SPACE 4, 4;
            
            STATICTEXT IDC_STATIC { NAME IDS_STATIC59; CENTER_V; ALIGN_LEFT; }
            COMBOBOX IDC_UPDATE_LICENSE_LANG_COMBO
            {
              ALIGN_TOP; SCALE_H; SIZE 150, 0; 
              CHILDS
              {
              }
            }
          }
          GROUP IDC_STATIC
          {
            SCALE_V; SCALE_H; 
            BORDERSTYLE BORDER_THIN_IN; BORDERSIZE 0, 0, 0, 0; 
            COLUMNS 1;
            SPACE 4, 4;
            
            HTMLVIEWER IDC_UPDATE_LICENSE_HTML_AREA
            {
              SCALE_V; SCALE_H; 
            }
          }
          CHECKBOX IDC_UPDATE_LICENSE_ACCEPT_CHK { NAME IDS_UPDATE_LICENSE_ACCEPT; ALIGN_TOP; ALIGN_LEFT;  }
        }
        GROUP IDC_UPDATE_DOWNLOAD_GROUP
        {
          NAME IDS_UPDATE_DOWNLOAD; SCALE_V; SCALE_H; 
          BORDERSIZE 0, 0, 0, 0; 
          COLUMNS 1;
          SPACE 4, 4;
          
          GROUP IDC_STATIC
          {
            NAME IDS_STATIC10; CENTER_V; SCALE_H; 
            BORDERSIZE 4, 4, 4, 4; 
            COLUMNS 1;
            SPACE 4, 4;
            
            STATICTEXT IDC_UPDATE_DOWNLOAD_WAIT_STATIC { NAME IDS_UPDATE_WAIT_DOWNLOAD; CENTER_V; SCALE_H; }
            STATICTEXT IDC_UPDATE_DOWNLOAD_CLOSE_STATIC { NAME IDS_STATIC34; CENTER_V; ALIGN_LEFT; }
            STATICTEXT IDC_STATIC { NAME IDS_STATIC12; CENTER_V; ALIGN_LEFT; }
            STATICTEXT IDC_DOWNLOAD_EXTRACT_STATIC { NAME IDS_UPDATE_DOWNLOADING; CENTER_V; SCALE_H; }
            STATICTEXT IDC_DOWNLOAD_EXTRACT_ITEM { NAME IDS_STATIC38; CENTER_V; ALIGN_LEFT; }
            GROUP IDC_UPDATE_DOWNLOAD_PROGRESS_BAR_GROUP
            {
              ALIGN_TOP; SCALE_H; 
              BORDERSTYLE BORDER_THIN_IN; BORDERSIZE 0, 0, 0, 0; 
              COLUMNS 1;
              SPACE 4, 4;
              
              PROGRESSBAR IDC_UPDATE_DOWNLOAD_PROGRESS_BAR
              {
                CENTER_V; SCALE_H; SIZE -100, -12; 
              }
            }
            GROUP IDC_STATIC
            {
              NAME IDS_STATIC27; ALIGN_TOP; SCALE_H; 
              BORDERSIZE 0, 0, 0, 0; 
              COLUMNS 2;
              SPACE 4, 4;
              
              STATICTEXT IDC_STATIC { NAME IDS_STATIC28; CENTER_V; ALIGN_LEFT; }
              STATICTEXT IDC_UPDATE_DOWNLOAD_TIME_LEFT { NAME IDS_STATIC25; CENTER_V; SCALE_H; }
              STATICTEXT IDC_UPDATE_DOWNLOAD_SPEED_LABEL { NAME IDS_STATIC29; CENTER_V; ALIGN_LEFT; }
              STATICTEXT IDC_UPDATE_DOWNLOAD_SPEED { NAME IDS_STATIC26; CENTER_V; SCALE_H; }
              STATICTEXT IDC_UPDATE_DOWNLOAD_SIZE_LABEL { NAME IDS_STATIC36; CENTER_V; ALIGN_LEFT; }
              STATICTEXT IDC_UPDATE_DOWNLOAD_SIZE { NAME IDS_STATIC37; CENTER_V; SCALE_H; }
            }
          }
          HTMLVIEWER IDC_UPDATE_DOWNLOAD_HTML
          {
            SCALE_V; SCALE_H; 
          }
        }
        GROUP IDC_UPDATE_INSTALL_GROUP
        {
          NAME IDS_UPDATE_INSTALL; SCALE_V; SCALE_H; 
          BORDERSIZE 0, 0, 0, 0; 
          COLUMNS 1;
          SPACE 4, 4;
          
          GROUP IDC_STATIC
          {
            NAME IDS_STATIC19; ALIGN_TOP; SCALE_H; 
            BORDERSIZE 0, 0, 0, 0; 
            COLUMNS 1;
            SPACE 4, 4;
            
            STATICTEXT IDC_INSTALL_DOWNLOAD_COMPLETED_STATIC { NAME IDS_UPDATE_DL_COMPLETE; CENTER_V; ALIGN_LEFT; }
            STATICTEXT IDC_INSTALL_RESTART_CINEMA_STATIC { NAME IDS_UPDATE_C4D_RESTART; CENTER_V; ALIGN_LEFT; }
            USERAREA IDC_UPDATE_INSTALL_USER_AREA { FIT_V; SCALE_H; SIZE 100, 100; }
            GROUP IDC_UPDATE_INSTALL_PROGRESS_BAR_GROUP
            {
              ALIGN_TOP; SCALE_H; 
              BORDERSTYLE BORDER_THIN_IN; BORDERSIZE 0, 0, 0, 0; 
              COLUMNS 1;
              SPACE 4, 4;
              
              PROGRESSBAR IDC_UPDATE_INSTALL_PROGRESS_BAR
              {
                CENTER_V; SCALE_H; SIZE -100, -12; 
              }
            }
            GROUP IDC_STATIC
            {
              NAME IDS_STATIC30; ALIGN_TOP; SCALE_H; 
              BORDERSIZE 0, 0, 0, 0; 
              ROWS 1;
              SPACE 4, 4;
              
              STATICTEXT IDC_STATIC { NAME IDS_STATIC31; CENTER_V; ALIGN_LEFT; }
              STATICTEXT IDC_UPDATE_INSTALL_TIME_LEFT { NAME IDS_STATIC24; CENTER_V; SCALE_H; }
            }
          }
          HTMLVIEWER IDC_UPDATE_INSTALL_HTML_AREA
          {
            SCALE_V; SCALE_H; 
          }
        }
        GROUP IDC_INSTALL_SUCCESS_GROUP
        {
          NAME IDS_STATIC63; SCALE_V; SCALE_H; 
          BORDERSIZE 0, 0, 0, 0; 
          COLUMNS 1;
          SPACE 4, 4;
          
          GROUP IDC_STATIC
          {
            NAME IDS_STATIC94; SCALE_V; SCALE_H; 
            BORDERSIZE 0, 40, 0, 0; 
            COLUMNS 1;
            SPACE 4, 20;
            
            USERAREA IDC_INSTALL_SUCCESS_BITMAP_AREA { CENTER_V; CENTER_H; SIZE 100, 100; }
            STATICTEXT IDC_STATIC { NAME IDS_STATIC65; CENTER_V; CENTER_H; }
            CHECKBOX IDC_INSTALL_SUCCESS_START_CINEMA_CHK { NAME IDS_INSTALL_SUCCESS_START_CINEMA; ALIGN_TOP; CENTER_H;  }
            GROUP IDC_INSTALL_SUCCESS_STUDENT_GROUP
            {
              NAME IDS_STATIC124; ALIGN_TOP; CENTER_H; 
              BORDERSIZE 0, 0, 0, 0; 
              COLUMNS 1;
              SPACE 4, 4;
              
              STATICTEXT IDC_STATIC { NAME IDS_STATIC125; CENTER_V; CENTER_H; }
              STATICTEXT IDC_STATIC { NAME IDS_STATIC126; CENTER_V; ALIGN_LEFT; }
              BUTTON IDC_INSTALL_SUCCESS_STUDENT_SHOW_README_BTN { NAME IDS_INSTALL_SUCCESS_STUDENT_SHOW_README; ALIGN_TOP; CENTER_H; }
            }
            STATICTEXT IDC_STATIC { NAME IDS_STATIC118; CENTER_V; SCALE_H; }
          }
        }
        GROUP IDC_INSTALL_FAILED_GROUP
        {
          NAME IDS_STATIC64; SCALE_V; SCALE_H; 
          BORDERSIZE 0, 0, 0, 0; 
          COLUMNS 1;
          SPACE 4, 4;
          
          GROUP IDC_STATIC
          {
            NAME IDS_STATIC111; SCALE_V; SCALE_H; 
            BORDERSIZE 0, 40, 0, 0; 
            COLUMNS 1;
            SPACE 4, 20;
            
            USERAREA IDC_INSTALL_FAILED_BITMAP_AREA { CENTER_V; CENTER_H; SIZE 100, 100; }
            STATICTEXT IDC_STATIC { NAME IDS_STATIC66; CENTER_V; CENTER_H; }
            STATICTEXT IDC_STATIC { NAME IDS_STATIC116; CENTER_V; SCALE_H; }
          }
        }
        GROUP IDC_UNINSTALL_GROUP
        {
          NAME IDS_STATIC82; CENTER_V; SCALE_H; 
          BORDERSIZE 0, 0, 0, 0; 
          COLUMNS 1;
          SPACE 4, 4;
          
          CHECKBOX IDC_UNINSTALL_DELETE_CHK { NAME IDS_UNINSTALL_DELETE; ALIGN_TOP; SCALE_H;  }
          GROUP IDC_STATIC
          {
            NAME IDS_STATIC121; ALIGN_TOP; SCALE_H; 
            BORDERSIZE 18, 0, 0, 0; 
            COLUMNS 1;
            SPACE 4, 4;
            
            GROUP IDC_STATIC
            {
              NAME IDS_STATIC109; ALIGN_TOP; SCALE_H; 
              BORDERSIZE 0, 10, 0, 30; 
              COLUMNS 1;
              SPACE 4, 4;
              
              STATICTEXT IDC_STATIC { NAME IDS_STATIC108; CENTER_V; ALIGN_LEFT; }
              FILENAME IDC_UNINSTALL_PATH_STATIC
              {
                ALIGN_TOP; SCALE_H; 
                DIRECTORY; 
                READONLY; 
              }
            }
            CHECKBOX IDC_UNINSTALL_DELETE_PREFS_CHK { NAME IDS_UNINSTALL_DELETE_PREFS; ALIGN_TOP; ALIGN_LEFT;  }
            GROUP IDC_STATIC
            {
              NAME IDS_STATIC113; ALIGN_TOP; SCALE_H; 
              BORDERSIZE 0, 10, 0, 30; 
              COLUMNS 1;
              SPACE 4, 4;
              
              STATICTEXT IDC_STATIC { NAME IDS_STATIC114; CENTER_V; ALIGN_LEFT; }
              FILENAME IDC_UNINSTALL_PREFS_PATH_STATIC
              {
                ALIGN_TOP; SCALE_H; 
                DIRECTORY; 
                READONLY; 
              }
            }
            CHECKBOX IDC_UNINSTALL_DELETE_PLUGINS_CHK { NAME IDS_UNINSTALL_DELETE_PLUGINS; ALIGN_TOP; SCALE_H;  }
          }
          GROUP IDC_UNINSTALL_PROGRESS_GROUP
          {
            NAME IDS_STATIC85; ALIGN_TOP; SCALE_H; 
            BORDERSIZE 0, 30, 0, 0; 
            COLUMNS 1;
            SPACE 4, 4;
            
            STATICTEXT IDC_STATIC { NAME IDS_STATIC86; CENTER_V; ALIGN_LEFT; }
            GROUP IDC_UNINSTALL_PROGRESS_BAR_GROUP
            {
              NAME IDS_STATIC88; ALIGN_TOP; SCALE_H; 
              BORDERSIZE 0, 0, 0, 0; 
              COLUMNS 1;
              SPACE 4, 4;
              
              PROGRESSBAR IDC_UNINSTALL_PROGRESS_BAR
              {
                CENTER_V; SCALE_H; SIZE -100, -12; 
              }
            }
          }
        }
      }
    }
    SEPARATOR { SCALE_H; }
    GROUP IDC_BUTTONS_GROUP
    {
      NAME IDS_STATIC8; FIT_V; CENTER_H; 
      BORDERSIZE 0, 0, 0, 0; 
      ROWS 1;
      EQUAL_COLS; 
      SPACE 4, 4;
      
      BUTTON IDC_UPDATE_CANCEL_BTN { NAME IDS_CANCEL_BTN; ALIGN_TOP; SCALE_H; SIZE 0, 14; }
      BUTTON IDC_UPDATE_BACK_BTN { NAME IDS_UPDATE_BACK; ALIGN_TOP; SCALE_H; SIZE 0, 14; }
      BUTTON IDC_UPDATE_CONTINUE_BTN { NAME IDS_UPDATE_CONTINUE; ALIGN_TOP; SCALE_H; SIZE 0, 14; }
    }
  }
}
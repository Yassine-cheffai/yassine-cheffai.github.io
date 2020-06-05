// C4D-DialogResource
DIALOG IDD_UV_MAP
{
  NAME IDS_DIALOG; FIT_V; SCALE_V; FIT_H; SCALE_H; 
  
  GROUP IDC_STATIC
  {
    NAME IDS_STATIC2; FIT_V; SCALE_V; FIT_H; SCALE_H; 
    BORDERSIZE 2, 2, 2, 2; 
    COLUMNS 1;
    SPACE 4, 4;
    
    QUICKTAB IDC_UV_MAP_MODE_TAB
    {
      ALIGN_TOP; FIT_H; SCALE_H; 
    }
    SCROLLGROUP IDC_UV_MAP_SCROLL
    {
      NAME IDS_STATIC; FIT_V; SCALE_V; FIT_H; SCALE_H; 
      SCROLL_V; SCROLL_H; SCROLL_AUTO_H; SCROLL_AUTO_V; 
      
      GROUP IDC_UV_MAP_DIALOG_CONTAINER
      {
        NAME IDS_STATIC1; FIT_V; SCALE_V; FIT_H; SCALE_H; 
        BORDERSIZE 0, 0, 0, 0; 
        COLUMNS 1;
        SPACE 4, 4;
        
        
      }
    }
  }
}
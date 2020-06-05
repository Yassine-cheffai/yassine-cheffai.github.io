// C4D-DialogResource

DIALOG DLG_SPACEMOUSE
{
  SCALE_H;

  GROUP {
    COLUMNS 1;
    SCALE_H;

    GROUP {
      COLUMNS 2;
      BORDERSIZE 6,4,6,0;
      CHECKBOX IDC_ENABLED { NAME DLGS_ENABLED; }
      CHECKBOX IDC_REDUCE_ANGLES { NAME DLGS_REDUCE_ANGLES; }
    }

    TAB IDC_MODES {
      SELECTION_TABS;
      SCALE_H;

      // mode NAVIGATE
	    GROUP IDC_NAVIGATE {
        NAME DLGS_TAB_NAVIGATE;
        COLUMNS 1;
        SCALE_H;

        GROUP {
          COLUMNS 1;
          BORDERSIZE 4,4,4,4;
          SCALE_H;

          GROUP {
            COLUMNS 4;
            SCALE_H;

            CHECKBOX IDC_NAVI_MOVE { NAME DLGS_MOVE; }
            CHECKBOX IDC_NAVI_MOVE_INV { SCALE_H; ALIGN_RIGHT; NAME DLGS_MOVE_INV; }
            GROUP {
              COLUMNS 4;
              BORDERSIZE 8,0,0,0;
              SCALE_H; ALIGN_RIGHT;
              CHECKBOX IDC_NAVI_MOVE_X { NAME DLGS_MOVE_X; }
              CHECKBOX IDC_NAVI_MOVE_Y { NAME DLGS_MOVE_Y; }
              CHECKBOX IDC_NAVI_MOVE_Z { NAME DLGS_MOVE_Z; }
              COMBOBOX IDC_NAVI_MOVE_SPACE {
                CHILDS {
                  IDC_SPACE_CAM, DLGS_CAM;
                  IDC_SPACE_AXIS, DLGS_AXIS;
                  IDC_SPACE_WORLD, DLGS_WORLD;
                }
              }
            }
          }

          EDITSLIDER IDC_NAVI_MOVE_SPEED { FIT_H; }
        }

        SEPARATOR { FIT_H; }

        GROUP {
          COLUMNS 1;
          BORDERSIZE 4,4,4,4;
          SCALE_H;

          GROUP {
            COLUMNS 3;
            SCALE_H;

            CHECKBOX IDC_NAVI_ROT { NAME DLGS_ROT; }
            CHECKBOX IDC_NAVI_ROT_INV { SCALE_H; ALIGN_RIGHT; NAME DLGS_ROT_INV; }
            GROUP {
              COLUMNS 4;
              BORDERSIZE 8,0,0,0;
              SCALE_H; ALIGN_RIGHT;
              CHECKBOX IDC_NAVI_ROT_H { NAME DLGS_ROT_H; }
              CHECKBOX IDC_NAVI_ROT_P { NAME DLGS_ROT_P; }
              CHECKBOX IDC_NAVI_ROT_B { NAME DLGS_ROT_B; }
              COMBOBOX IDC_NAVI_ROT_SPACE {
                CHILDS {
                  IDC_SPACE_CAM, DLGS_CAM;
                  IDC_SPACE_AXIS, DLGS_AXIS;
                  IDC_SPACE_WORLD, DLGS_WORLD;
                }
              }
            }
          }

          EDITSLIDER IDC_NAVI_ROT_SPEED { FIT_H; }
        }
      }

      // mode FLY
	    GROUP IDC_FLY {
        NAME DLGS_TAB_FLY;
        COLUMNS 1;
        SCALE_H;

        GROUP {
          COLUMNS 1;
          BORDERSIZE 4,4,4,4;
          SCALE_H;

          GROUP {
            COLUMNS 3;
            SCALE_H;

            CHECKBOX IDC_FLY_MOVE { NAME DLGS_MOVE; }
            CHECKBOX IDC_FLY_MOVE_INV { SCALE_H; ALIGN_RIGHT; NAME DLGS_MOVE_INV; }
            GROUP {
              COLUMNS 4;
              BORDERSIZE 8,0,0,0;
              SCALE_H; ALIGN_RIGHT;
              CHECKBOX IDC_FLY_MOVE_X { NAME DLGS_MOVE_X; }
              CHECKBOX IDC_FLY_MOVE_Y { NAME DLGS_MOVE_Y; }
              CHECKBOX IDC_FLY_MOVE_Z { NAME DLGS_MOVE_Z; }
              COMBOBOX IDC_FLY_MOVE_SPACE {
                CHILDS {
                  IDC_SPACE_CAM, DLGS_CAM;
                  IDC_SPACE_AXIS, DLGS_AXIS;
                  IDC_SPACE_WORLD, DLGS_WORLD;
                }
              }
            }
          }

          EDITSLIDER IDC_FLY_MOVE_SPEED { FIT_H; }
        }

        SEPARATOR { FIT_H; }

        GROUP {
          COLUMNS 1;
          BORDERSIZE 4,4,4,4;
          SCALE_H;

          GROUP {
            COLUMNS 3;
            SCALE_H;

            CHECKBOX IDC_FLY_ROT { NAME DLGS_ROT; }
            CHECKBOX IDC_FLY_ROT_INV { SCALE_H; ALIGN_RIGHT; NAME DLGS_ROT_INV; }
            GROUP {
              COLUMNS 4;
              BORDERSIZE 8,0,0,0;
              SCALE_H; ALIGN_RIGHT;
              CHECKBOX IDC_FLY_ROT_H { NAME DLGS_ROT_H; }
              CHECKBOX IDC_FLY_ROT_P { NAME DLGS_ROT_P; }
              CHECKBOX IDC_FLY_ROT_B { NAME DLGS_ROT_B; }
              COMBOBOX IDC_FLY_ROT_SPACE {
                CHILDS {
                  IDC_SPACE_CAM, DLGS_CAM;
                  IDC_SPACE_AXIS, DLGS_AXIS;
                  IDC_SPACE_WORLD, DLGS_WORLD;
                }
              }
            }
          }

          EDITSLIDER IDC_FLY_ROT_SPEED { FIT_H; }
        }
      }

      // mode MOVE
	    GROUP IDC_MOVE {
        NAME DLGS_TAB_MOVE;
        COLUMNS 1;
        SCALE_H;

        GROUP {
          COLUMNS 1;
          BORDERSIZE 4,4,4,4;
          SCALE_H;

          GROUP {
            COLUMNS 3;
            SCALE_H;

            CHECKBOX IDC_MOVE_MOVE { NAME DLGS_MOVE; }
            CHECKBOX IDC_MOVE_MOVE_INV { SCALE_H; ALIGN_RIGHT; NAME DLGS_MOVE_INV; }
            GROUP {
              COLUMNS 4;
              BORDERSIZE 8,0,0,0;
              SCALE_H; ALIGN_RIGHT;
              CHECKBOX IDC_MOVE_MOVE_X { NAME DLGS_MOVE_X; }
              CHECKBOX IDC_MOVE_MOVE_Y { NAME DLGS_MOVE_Y; }
              CHECKBOX IDC_MOVE_MOVE_Z { NAME DLGS_MOVE_Z; }
              COMBOBOX IDC_MOVE_MOVE_SPACE {
                CHILDS {
                  IDC_SPACE_CAM, DLGS_CAM;
                  IDC_SPACE_AXIS, DLGS_AXIS;
                  IDC_SPACE_WORLD, DLGS_WORLD;
                }
              }
            }
          }

          EDITSLIDER IDC_MOVE_MOVE_SPEED { FIT_H; }
        }

        SEPARATOR { FIT_H; }

        GROUP {
          COLUMNS 1;
          BORDERSIZE 4,4,4,4;
          SCALE_H;

          GROUP {
            COLUMNS 3;
            SCALE_H;

            CHECKBOX IDC_MOVE_ROT { NAME DLGS_ROT; }
            CHECKBOX IDC_MOVE_ROT_INV { SCALE_H; ALIGN_RIGHT; NAME DLGS_ROT_INV; }
            GROUP {
              COLUMNS 4;
              BORDERSIZE 8,0,0,0;
              SCALE_H; ALIGN_RIGHT;
              CHECKBOX IDC_MOVE_ROT_H { NAME DLGS_ROT_H; }
              CHECKBOX IDC_MOVE_ROT_P { NAME DLGS_ROT_P; }
              CHECKBOX IDC_MOVE_ROT_B { NAME DLGS_ROT_B; }
              COMBOBOX IDC_MOVE_ROT_SPACE {
                CHILDS {
                  IDC_SPACE_CAM, DLGS_CAM;
                  IDC_SPACE_AXIS, DLGS_AXIS;
                  IDC_SPACE_WORLD, DLGS_WORLD;
                }
              }
            }
          }

          EDITSLIDER IDC_MOVE_ROT_SPEED { FIT_H; }
        }
      }
    }
  }
}

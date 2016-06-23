#include "user.h"



		//对话框资源

const GUI_WIDGET_CREATE_INFO ExampleDialog[] =            
{
  { FRAMEWIN_CreateIndirect,  "实时生产管理系统", 1000,                   	0,  0, 240, 320, 0, 0 }, 
  { TEXT_CreateIndirect,  	"程序主界面", GUI_ID_TEXT0,       			0,  10, 234,  30, 0},
  { BUTTON_CreateIndirect,  "生产入数", GUI_ID_BUTTON0,       			10,  50, 90,  60, 0},
  { BUTTON_CreateIndirect,  "停工记录", GUI_ID_BUTTON1,             	10,130, 90,  60, 0}, 
  { BUTTON_CreateIndirect,  "工序交接", GUI_ID_BUTTON2,             	10,210, 90,  60, 0}, 
  { BUTTON_CreateIndirect,	"打印日报表", GUI_ID_BUTTON3, 				130,50, 90,  60, 0}, 
  { BUTTON_CreateIndirect,	"半成品标识", GUI_ID_BUTTON4, 				130,130, 90,  60, 0}, 
  { BUTTON_CreateIndirect,	"退出程序", GUI_ID_BUTTON5, 				130,210, 90,  60, 0}, 

};


//对资源控件属性初始化
void ExampleInitDialog( WM_HWIN hDlg ){
		WM_HWIN pObj;

    pObj = WM_GetDialogItem( hDlg, GUI_ID_TEXT0);

		TEXT_SetTextAlign(pObj,GUI_TA_CENTER);
		TEXT_SetFont(pObj,GUI_GetFontAddress(24));
	
  	WM_MakeModal( hDlg );                         //设置对话框为独占模式
  	WM_SetFocus( hDlg );                          //将系统焦点定位到对话框内
}



//回调函数，处理触摸屏和按键消息
void ExampleCbFuntion( WM_MESSAGE* pMsg ){
  int NCode, Id;
  WM_HWIN hDlg;

  hDlg = pMsg->hWin;

  switch ( pMsg->MsgId )
  {
    case WM_INIT_DIALOG:
      ExampleInitDialog( hDlg );

      break;
    case WM_PAINT:
      break;  
    case WM_NOTIFY_PARENT:
      Id = WM_GetId( pMsg->hWinSrc );   
      NCode = pMsg->Data.v;        
      switch ( NCode )
      {
        case WM_NOTIFICATION_RELEASED: //触摸屏消息
         
          switch ( Id )
          {
            case GUI_ID_BUTTON0:
							GUI_EndDialog( hDlg, GUI_ID_BUTTON0 ); 
              break;
						case GUI_ID_BUTTON1:
							GUI_EndDialog( hDlg, GUI_ID_BUTTON1 ); 
							break;
            case GUI_ID_BUTTON2:
              GUI_EndDialog( hDlg, GUI_ID_BUTTON2 ); 
              break;
						case GUI_ID_BUTTON3:
							GUI_EndDialog( hDlg, GUI_ID_BUTTON3 ); 
							break;
									case GUI_ID_BUTTON4:
							GUI_EndDialog( hDlg, GUI_ID_BUTTON4 ); 
										break;
						case GUI_ID_BUTTON5:
							GUI_EndDialog( hDlg, GUI_ID_BUTTON5 ); 
							break;

          }
          break;
      }
      break;

    case WM_KEY: //按键消息
     
      switch ( ( ( WM_KEY_INFO * )( pMsg->Data.p ) )->Key )
      {
        case GUI_KEY_ESCAPE:
          GUI_EndDialog( hDlg, -1 );
          break;
				case GUI_KEY_NUM1:
					GUI_EndDialog( hDlg, GUI_ID_BUTTON0 ); 
					break;
				case GUI_KEY_NUM2:
					GUI_EndDialog( hDlg, GUI_ID_BUTTON1 ); 
					break;
				case GUI_KEY_NUM3:
					GUI_EndDialog( hDlg, GUI_ID_BUTTON2 ); 
					break;
				case GUI_KEY_NUM4:
					GUI_EndDialog( hDlg, GUI_ID_BUTTON3 ); 
					break;
				case GUI_KEY_NUM5:
					GUI_EndDialog( hDlg, GUI_ID_BUTTON4 ); 
					break;
				case GUI_KEY_NUM6:
					GUI_EndDialog( hDlg, GUI_ID_BUTTON5 ); 
					break;
				case GUI_KEY_UP:
					WM_SetFocusOnPrevChild(WM_GetParent(WM_GetDialogItem( hDlg, GUI_ID_BUTTON0)));
					break;
				case GUI_KEY_DOWN:
					WM_SetFocusOnNextChild(WM_GetParent(WM_GetDialogItem( hDlg, GUI_ID_BUTTON0)));
					break;
      }
      break;

    default:
      WM_DefaultProc( pMsg );
  }
} 


int maingui( void ){
	int iRet;
	while(1){
  		iRet=GUI_ExecDialogBox( ExampleDialog, GUI_COUNTOF( ExampleDialog ), &ExampleCbFuntion, WM_HBKWIN, 0, 0 );//创建并执行对话框
  		switch(iRet){
			case GUI_ID_BUTTON0:
				break;
			case GUI_ID_BUTTON1:
				break;
			case GUI_ID_BUTTON2:
				break;
			case GUI_ID_BUTTON3:
				break;
			case GUI_ID_BUTTON4:
				break;
			case GUI_ID_BUTTON5:
				return 0;
			case -1:
				return -1;
			default:
				break;
			}
  }
}

int main(void){

	U8 key;
	LcdClear();	
	while(1){
		LcdPutString("Hello......",0,16);
		key=KeyWait();
		if(key==KEY_ESC) break;
		else if(key==KEY_ENTER) maingui();
	}
	return 0;
		}

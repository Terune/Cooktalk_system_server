// Tab3.cpp : 구현 파일입니다.
//

#include "main.h"


// CTab3 대화 상자입니다.
int main(int argc,char *argv[]){
   CTab3 tab;
   string input_act;
   string input_slot;
   string input_value;
   char input_ma[55];
   if(tab.OnInitDialog())
   {
      strcpy(input_ma,argv[1]);
      for(int i=0;i<55;i++)
      {
         tab.Materials[i]=input_ma[i]-'0';
      }
     tab.slots["Menu"]=string(argv[2]);
     tab.slots["Type"]=string(argv[3]);
     tab.slots["Con"]=string(argv[4]);
      
tab.By=string(argv[5]);
      
      input_act=string(argv[6]);
      input_slot=string(argv[7]);
      input_value=string(argv[8]);
      //tab.now_act.push(string(argv[6]));
      //tab.now_slot.push(string(argv[7]));
      //tab.now_value.push(string(argv[8]));
      tab.input_DA=string(argv[9]);
      tab.input_slot=string(argv[10]);
      tab.input_value=string(argv[11]);

	if(tab.input_slot=="menu")
		tab.input_slot="Menu";
	if(tab.input_slot=="type")
		tab.input_slot="Type";
	if(tab.input_slot=="con")
		tab.input_slot="Con";
	if(tab.input_slot=="material")
		tab.input_slot="Material";
      tab.stack_input(input_act,"now_act");
      tab.stack_input(input_slot,"now_slot");
      tab.stack_input(input_value,"now_value");
      for(int i=0;i<12;i++)
      {
         string DA_NAME[12]={"end","goback","restart","null","repeat","affirm","negate",
            "alarm_start","movie_start","heat_start","next","reqalts"};
         if(tab.input_DA.compare(DA_NAME[i])==0)
            tab.Check_Input[i]=1;
      }
      
      if(tab.input_DA=="inform")
      {
         tab.inform_In_slot=tab.input_slot;
         tab.inform_In_value=tab.input_value;
      
   tab.inform_add();

      }
      else if(tab.input_DA=="confirm")
      {
         tab.Confirm_In_slot=tab.input_slot;
         tab.Confirm_In_value=tab.input_value;
         tab.confirm_add();
      }
      else if(tab.input_DA=="deny")
      {
         tab.deny_In_slot=tab.input_slot;
         tab.deny_In_value=tab.input_value;
         tab.deny_add();
      }
      else if(tab.input_DA=="request")
      {
         tab.request_Get_name(tab.input_slot);
      }
 tab.act_working();
      cout<<"Materials:";
      for(int i=0;i<55;i++)
         cout<<tab.Materials[i];
      cout<<"\nMenu:"<<tab.slots.find("Menu")->second<<"\n";
      cout<<"Type:"<<tab.slots.find("Type")->second<<"\n";
      cout<<"Con:"<<tab.slots.find("Con")->second<<"\n";
      cout<<"By:"<<tab.By<<"\n";
      cout<<"now_act:"<<tab.now_act.top()<<"\n";
      cout<<"now_slot:";
      if(!tab.now_slot.empty())
         cout<<tab.now_slot.top();
      cout<<"\nnow_value:";
      if(!tab.now_value.empty())
         cout<<tab.now_value.top();
      cout<<"\nnow_DA:"<<tab.input_DA<<"\n";
      cout<<"now_slot:"<<tab.input_slot<<"\n";
      cout<<"now_value:"<<tab.input_value<<"\n";

      cout<<tab.output<<endl;
   }
   tab.postmessage();
   return 0;
}
void CTab3::stack_input(string input,string type)
{
   char *dup;
   char *pch;
   char *bp;
   Stack<string> temp;
  
   dup=strdup(input.c_str());
      pch=strtok_r(dup,"/",&bp);
      while(pch!=NULL)
      {
         if(strcmp(pch,"none")!=0)
            temp.push(string(pch));
         pch=strtok_r(NULL,"/",&bp);
      }
      for(int i=0;0<temp.size();i++)
      {
         if(type=="now_act")
            now_act.push(temp.top());
          else if(type=="now_slot")
            now_slot.push(temp.top());
          else if(type=="now_value")
            now_value.push(temp.top());
         temp.pop();
      }
}
void CTab3::postmessage()
{
   CURL *curl;
   CURLcode res;
   curl_global_init(CURL_GLOBAL_ALL);
  // char outputs[200];
   string outputs;
   stringstream ss;
   outputs="Materials=";
   for(int i=0;i<55;i++)
   {
      ss<<Materials[i];
   }

   string act_output;
   string slot_output;
   string value_output;
   outputs+=ss.str();

   for(int i=0;0<now_act.size();i++)
   {
      act_output+=now_act.top()+"/";
      now_act.pop();
   }
   for(int i=0;0<now_slot.size();i++)
   {
     slot_output+=now_slot.top()+"/";
     now_slot.pop();
   }
   for(int i=0;0<now_value.size();i++)
   {
      value_output+=now_value.top()+"/";
      now_value.pop();
   }
   cout<<"actout:"<<act_output<<endl;
   cout<<"slotout:"<<slot_output<<endl;
   cout<<"valueout:"<<value_output<<endl;
   if(act_output=="")
	act_output="none";
   if(slot_output=="")
	slot_output="none";
   if(value_output=="")
	value_output="none";
   outputs+="&Menu="+slots.find("Menu")->second+"&Type="+slots.find("Type")->second+"&Con="+slots.find("Con")->second+"&By="+By+"&now_act="+act_output+"&now_slot="+slot_output+"&now_value="+value_output+"&output_DA="+output_DA+"&output_m="+output;
   
curl_global_init(CURL_GLOBAL_ALL);
	curl=curl_easy_init();
   if(curl){
      curl_easy_setopt(curl,CURLOPT_URL,"http://localhost:5000/getpro");

      curl_easy_setopt(curl,CURLOPT_POSTFIELDS,outputs.c_str());
      curl_easy_setopt(curl,CURLOPT_TIMEOUT,5);
      res=curl_easy_perform(curl);
      if(res!=CURLE_OK)
         fprintf(stderr,"curl_easy_perform() failed: %s\n",
               curl_easy_strerror(res));
      curl_easy_cleanup(curl);
   }
   
   curl_global_cleanup();
 
}

bool CTab3::OnInitDialog()
{
   for(int i=0;i<12;i++)
      Check_Input[i]=0;
   for(int i=0;i<55;i++)
      Materials[i]=0;
   for(int i=0;i<25;i++)
   {
      material_point[i]=0;
      con_point[i]=0;
      type_point[i]=0;
   }
   //User_input=_T("");
   Denyed=false;
   Slot_input="";
   output_DA="start";
   //SetDlgItemTextW(IDC_EDIT4,TEXT("start"));
   now_act.push("start");
   //before_act=TEXT("start");
   //SetDlgItemTextW(IDC_EDIT3,TEXT("LG Dios 광파오븐 제어기 입니다. 무엇을 도와드릴까요?"));
   output="잘못된 입력입니다. 다시 한번 말씀해 주십시오.";
   return true;
}
//deny working
void CTab3::deny_Get_name(string ComboName)
{

   //ClickList=0;

   string Get_name;


      //SetDlgItemTextW(IDC_EDIT1,ComboName+L(": ")+Get_name);

      if(Get_name.length()>0)
      {
         deny_In_slot=ComboName;
         deny_In_value=Get_name;
      }

}
//inform working
void CTab3::inform_Get_name(string ComboName)
{

   string Get_name;

     // SetDlgItemTextW(IDC_EDIT5,ComboName+TEXT(": ")+Get_name);

      if(Get_name.length()>0)
      {
         inform_In_slot=ComboName;
         inform_In_value=Get_name;
      }
}

//초기화
void CTab3::click_init()
{
   //SetDlgItemTextW(IDC_EDIT4,TEXT("start"));
   //SetDlgItemTextW(IDC_EDIT3,TEXT("LG Dios 광파오븐 제어기 입니다. 무엇을 도와드릴까요?"));
   //output_DA="start";
   //output="LG Dios 광파오븐 제어기입니다. 무엇을 도와드릴까요?";
   //slots.clear();
   slots["Menu"]="none";
   slots["Con"]="none";
   slots["Type"]="none";
   for(int i=0;i<55;i++)
      Materials[i]=0;
   for(int i=0;i<25;i++)
   {
      material_point[i]=0;
      con_point[i]=0;
      type_point[i]=0;
   }
   for(int i=0;0<now_slot.size();i++)
      now_slot.pop();
   for(int i=0;0<now_act.size();i++)
      now_act.pop();
   for(int i=0;0<now_value.size();i++)
      now_value.pop();

   //SetDlgItemTextW(IDC_EDIT2,TEXT("Goal_label(Material:/ Menu:/ Con:/ Type:), Method_label(), Request_label()"));
   Slot_input="";
   now_act.push("start");
   print_DA="start";
   Requested=false;
   Confirmed=false;
   Denyed=false;
   Request_slot="";
   ViewNowValue();
   // 초기화 버튼 클릭 이벤트
}

// deny add
void CTab3::deny_add()
{
   if(deny_In_slot.length()>0)
   {
      string Get_value;
      if(deny_In_slot=="Material")
      {
         Materials[returnMaterialNum(deny_In_value)]=-1;
         Denyed=true;
         
      }
      else
      {
         Get_value=slots.find(deny_In_slot)->second;
         if(Get_value!="")
         {
            if(Get_value==deny_In_value)
            {
               Denyed=true;
               slots.erase(deny_In_slot);
            }
         }
      }

      if(Slot_input.length()>0)
         Slot_input+=", ";
      Slot_input+="deny("+deny_In_slot+":"+deny_In_value+")";

      //deny_In_slot="");
      //deny_In_value="");
   }
   ViewNowValue();
}


int CTab3::returnMaterialNum(string input)
{
   string Material_names[55]={"eggplant","potato","cinnamon_powder","sweet_potato","honey","pizza",
      "pork","crushed_galic","beef","small_green_onion","egg","drumstick","jujube","pork_rib",
      "Tortilla","nonglutinous_rice","cheese","vanilla_oil","soft_flour","chestnut","butter",
      "bagle","bacon","baking_soda","baking_powder","black_olive","chicken","cream","spaghetti",
      "bread","rice","button_mushroom","onion","oregano","oligosaccharides",
      "worcester_sauce","bay_leaf","meat_broth","ginkgo_nut","pine_nut","red_wine",
      "medium_flour","sticky_rice","squid","chocochip","bean","cream_cheese",
      "tomato_paste","pork_belly","parmesan","parsley","bread_crumbs","pizza_source",
      "ham","red_peppers"};

   for(int i=0;i<55;i++)
      if(Material_names[i]==input)
         return i;
}

//inform add
void CTab3::inform_add()
{
   if(inform_In_slot.length()>0&&inform_In_slot!="none"&&inform_In_value!="none")
   {
      string Get_value;
      if(inform_In_slot=="Material")
      {
         Materials[returnMaterialNum(inform_In_value)]=1;
      }
      else
      {
	      Get_value=slots.find(inform_In_slot)->second;
	      if(Get_value!="")
	      {
		      slots.erase(inform_In_slot);
	      }
	      slots[inform_In_slot]=inform_In_value;
      }

      if(Slot_input.length()>0)
         Slot_input+=", ";
      Slot_input+="inform("+inform_In_slot+":"+inform_In_value+")";

      inform_In_slot="";
      inform_In_value="";
   }
   ViewNowValue();
   // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

//confirm working
void CTab3::confirm_Get_name()
{

   string Get_name;

      //SetDlgItemTextW(IDC_EDIT6,ComboName+TEXT(": ")+Get_name);
      if(Get_name.length()>0)
      {
         Confirm_In_slot=ComboName;
         Confirm_In_value=Get_name;
      }

}

void CTab3::ViewNowValue()
{
   string DA_NAME[12]={"end","goback","restart","null","repeat","affirm","negate",
      "alarm_start","movie_start","heat_start","next","reqalts"};

   string User_input="";
   bool first=true;
   for(int i=0;i<12;i++)
   {
      if(Check_Input[i]==1){
         if(first==true)
         {
            User_input+=DA_NAME[i];
            first=false;
         }
         else
            User_input+=", "+DA_NAME[i];
      }
   }

   if(first==true)
   {
      User_input+=Slot_input;
      first=false;
   }
   else
      User_input+=", "+Slot_input;
   
  // SetDlgItemTextW(IDC_EDIT7,User_input);
}
//request getname
void CTab3::request_Get_name(string ComboName)
{

   if(Slot_input.length()>0)
      Slot_input+=", ";
   Slot_input+="request("+ComboName+")";
   Request_slot=ComboName;
   Requested=true;
   ViewNowValue();
}

//confirm add
void CTab3::confirm_add()
{
   if(Slot_input.length()>0)
      Slot_input+=", ";
   Slot_input+="Confirm("+Confirm_In_slot+":"+Confirm_In_value+")";
   ViewNowValue();
}

//확인버튼
void CTab3::act_working()
{
   string DA_NAME[12]={"end","goback","restart","null","repeat","affirm","negate",
      "alarm_start","movie_start","heat_start","next","reqalts"};
   string slot_material;
   string slot_menu;
   string slot_con;
   string slot_type;
   bool Use_material=false;
   //BOOL first=true;
   if(Check_Input[0]==1)//end input
      Doact("end");
   else if(Check_Input[4]==1)//repeat input
      Doact("repeat");
   else if(Check_Input[1]==1)//goback input
      Doact("goback");

   else if(Check_Input[2]==1)
	   Doact("restart");
   else{
	   for(int i=0;i<55;i++)
	   {
		   if(Materials[i]==1)
			   Use_material=true;
	   }
	   slot_type=slots.find("Type")->second;
	   slot_con=slots.find("Con")->second;
	   slot_menu=slots.find("Menu")->second;
	   if(!now_act.top().compare("request")||!now_act.top().compare("start")||!now_act.top().compare("start_example")||!now_act.top().compare("con_example")||!now_act.top().compare("type_example")||(Denyed&&deny_In_slot.compare("Material")))
	   {
		   By="none";
		   if(Use_material)
			   By="bymaterial";
		   if(slot_type!="none")
			   By="bytype";
		   if(slot_con!="none")
			   By="bycon";
		   if(slot_menu!="none")
			   By="bymenu";
		   if(By=="none"){

			   if(Check_Input[3]==1)//null input
				   Doact("null");
			   if(Check_Input[5]==1)//affirm input
				   Doact("affirm");
			   if(Check_Input[6]==1)//negate input
				   Doact("negate");
			   if(Check_Input[7]==1)//negate input
				   Doact("alarm_start");
			   if(Check_Input[8]==1)//negate input
				   Doact("movie_start");
			   if(Check_Input[9]==1)//negate input
				   Doact("heat_start");
			   if(Check_Input[10]==1)//negate input
				   Doact("next");
			   if(Check_Input[11]==1)//negate input
				   Doact("reqalts");
		   }
		   else if(By=="bymaterial")
		   {
			   Doact("explain","material");
			   Doact("request","Menu");
		   }
		   else if(By=="bymenu")
		   {
			   Doact("impl_confirm","Menu",slot_menu);
			   Doact("explain","Menu",slot_menu);
			   Doact("recipe_q");
		   }
		   else if(By=="bycon")
		   {
			   Doact("explain","Con",slot_con);
			   Doact("request","Menu");
		   }
		   else if(By=="bytype")
		   {
			   Doact("explain","Type",slot_type);
			   Doact("request","Menu");
		   }
		   if(By=="none")
		   {
			   Doact("start_example");
			   Denyed=false;
		   }
	   }
	   else if(now_act.top()=="explain")
	   {
		   slot_menu=slots.find("Menu")->second;
		   if(slot_menu!="none")
		   {
			   Doact("impl_confirm","Menu",slot_menu);
			   Doact("explain","Menu",slot_menu);
			   Doact("recipe_q");
		   }
		   if(Check_Input[3]==1)//null input
			   Doact("null");
		   if(Check_Input[5]==1)//affirm input
			   Doact("affirm");
		   if(Check_Input[6]==1)//negate input
			   Doact("negate");
		   if(Check_Input[7]==1)//negate input
			   Doact("alarm_start");
		   if(Check_Input[8]==1)//negate input
			   Doact("movie_start");
		   if(Check_Input[9]==1)//negate input
			   Doact("heat_start");
		   if(Check_Input[10]==1)//negate input
			   Doact("next");
		   if(Check_Input[11]==1)//negate input
			   Doact("reqalts");
	   }
	   else
	   {

		   if(Check_Input[3]==1)//null input
			   Doact("null");
		   if(Check_Input[5]==1)//affirm input
		   {
			   Doact("affirm");
		   }
		   if(Check_Input[6]==1)//negate input
			   Doact("negate");
		   if(Check_Input[7]==1)//negate input
			   Doact("alarm_start");
		   if(Check_Input[8]==1)//negate input
			   Doact("movie_start");
		   if(Check_Input[9]==1)//negate input
			   Doact("heat_start");
		   if(Check_Input[10]==1)//negate input
			   Doact("next");
	   }

	   if(Check_Input[3]==1)//null input
		   Doact("null");
	   if(Check_Input[2]==1)//restart input
		   Doact("restart");
	   if(Requested)
	   {
		   // SetDlgItemTextW(IDC_EDIT2,"Goal_labels(Material:"+Find_Material("english")+"/ Menu:"+slot_menu+"/ Con:"+slot_con+"/ Type:"+slot_type+"), Method_label("+By+"), Requested_slot("+Request_slot+")");
		   Doact("request");
		   Requested=false;
	   }
	   //else
	   //SetDlgItemTextW(IDC_EDIT2,"Goal_labels(Material:"+Find_Material("english")+"/ Menu:"+slot_menu+"/ Con:"+slot_con+"/ Type:"+slot_type+"), Method_label("+By+"), Requested_slot()");
	   if(Denyed)
	   {
		   if(deny_In_slot=="Material")
		   {
			   Doact("impl_confirm","Material");
			   Doact("explain","material");
		   }
		   Denyed=false;

	   }
	   Slot_input="";
	   ViewNowValue();
   }
   // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
void CTab3::Doact(string act,string Slot,string Value)
{
   //CString print_act;
   if(act=="explain"&&Slot=="Menu")
   {
      now_act.push("explain");
      now_slot.push(Slot);
      now_value.push(Value);
      print_act+=Conv_Korean(Value)+"는 "+Menu_Material(Conv_Korean(Value))+"가 재료로 필요합니다.";
      print_DA+="explain(menu:"+Value+")";
      output_DA=print_DA;
      output=print_act;
     // SetDlgItemTextW(IDC_EDIT4,print_DA);
      //SetDlgItemTextW(IDC_EDIT3,print_act);
   }
   else if(act=="explain"&&Slot=="Con")
   {
      now_act.push("explain");
      now_slot.push(Slot);
      now_value.push(Value);
      print_act=Conv_Korean(Value)+"으로 할 수 있는 요리는 "+Menu_Con(Value)+"가 있습니다.";
      output_DA=print_DA;
      output=print_act;
      //SetDlgItemTextW(IDC_EDIT4,"explain(Con:"+Value+")");
      //SetDlgItemTextW(IDC_EDIT3,print_act);
   }
   else if(act=="explain"&&Slot=="Type")
   {
      now_act.push("explain");
      now_slot.push(Slot);
      now_value.push(Value);
      print_act=Conv_Korean(Value)+"로 할 수 있는 요리는 "+Menu_Type(Value)+"가 있습니다.";
     output_DA="explain(Type:"+Value+")";
     output=print_act;
      // SetDlgItemTextW(IDC_EDIT4,"explain(Type:"+Value+")");
     // SetDlgItemTextW(IDC_EDIT3,print_act);
   }
   else if(act=="impl_confirm"&&Slot=="Menu")
   {
      print_DA="impl_confirm(Menu:"+Value+")";
      print_act="요리로 "+Conv_Korean(Value)+"를 선택하였습니다.";
   
   }
   else if(act=="impl_confirm"&&Slot=="Con")
   {
      print_act="부속품으로 "+Conv_Korean(Value)+"을 선택하였습니다.";
   }
   else if(act=="impl_confirm"&&Slot=="Material")
   {
      print_act="재료로 "+Find_Material("Korean")+"를 선택하였습니다.";
   }
   else if(act=="impl_confirm"&&Slot=="Type")
   {
      print_act="종류로 "+Conv_Korean(Value)+"를 선택하였습니다.";
   }
   else if(act=="recipe")
   {
      string info;
      info=Find_recipe(Slot,atoi(Value.c_str()));
      if(info!="end")
      {
         output=info;
         //SetDlgItemTextW(IDC_EDIT3,info);
         now_act.push(act);
         now_slot.push(Slot);
         now_value.push(Value);
         output_DA="recipe("+Slot+":"+Value+")";
         //SetDlgItemTextW(IDC_EDIT4,"recipe("+Slot+":"+Value+")");
      }
      else
         Doact("recipe_end");
   }
   
}
void CTab3::Doact(string act,string Slot)
{
   //CString print_act;
   if(act=="explain")
   {
      now_act.push("explain");
      now_slot.push(Slot);
      print_act=Find_Material("korean")+"으로 만들수 있는 요리는 "+Find_menu()+"가 있습니다.";
      output_DA="explain(material:"+Find_Material("english")+")";
      output=print_act;
      //SetDlgItemTextW(IDC_EDIT4,"explain(material:"+Find_Material("english")+")");
      //SetDlgItemTextW(IDC_EDIT3,print_act);
   }
   	else if(act=="request")
	{
		now_act.push(act);
		now_slot.push(Slot);
		if(Slot=="Menu")
		{
			print_act+="어떤 요리를 안내해드릴까요?";
			print_DA+="request(Menu)";
			output_DA=print_DA;
         output=print_act;
         //SetDlgItemTextW(IDC_EDIT4,print_DA);
			//SetDlgItemTextW(IDC_EDIT3,print_act);
		}
		else if(Slot=="Material")
		{
			print_act+="무슨 재료를 사용할 것인가요?";
			print_DA+="request(Material)";
			output_DA=print_DA;
         output=print_act;
         //SetDlgItemTextW(IDC_EDIT4,print_DA);
			//SetDlgItemTextW(IDC_EDIT3,print_act);
		}
		else if(Slot=="Con")
		{
			print_act+="법량접시, 석쇠, 구이전용팬중 어떤 부속품을 사용하실건가요?";
			print_DA+="request(Con)";
			output_DA=print_DA;
         output=print_act;
         //SetDlgItemTextW(IDC_EDIT4,print_DA);
			//SetDlgItemTextW(IDC_EDIT3,print_act);
		}
		else if(Slot=="Type")
		{
			print_act+="토스트, 오븐 모드 중 어떤 모드를 선택할 것인지요?";
			print_DA+="request(Type)";
			output_DA=print_DA;
         output=print_act;
         //SetDlgItemTextW(IDC_EDIT4,print_DA);
			//SetDlgItemTextW(IDC_EDIT3,print_act);
		}
	}
      
}
void CTab3::Doact(string act)
{
   
   if(act=="end")
   {
      //before_act=now_act;
      now_act.push(act);
      output_DA="end";
      output="프로그램을 종료합니다.";
      //SetDlgItemTextW(IDC_EDIT4,"end");
      //SetDlgItemTextW(IDC_EDIT3,"프로그램을 종료합니다.");
   }
   else if(act=="request")
   {
      //now_act.push(act);
      string slot_con;
      string slot_type;
      if(Request_slot=="Menu")
      {
         if(By=="bymaterial")
         {
            Doact("explain","Material");
            Doact("request","Menu");
         }
         else if(By=="bytype")
         {
           slot_type=slots.find("Type")->second;
           // slots.Lookup("Type",slot_type);
            Doact("explain","Type",slot_type);
            Doact("request","Menu");
         }
         else if(By=="bycon")
         {
            slot_con=slots.find("Con")->second;
            //slots.Lookup("Con",slot_con);
            Doact("explain","Type",slot_con);
            Doact("request","Menu");
         }
         else if(By=="menu")
         {
            bool Use_material=false;
            for(int i=0;i<55;i++)
            {
               if(Materials[i]==1)
                  Use_material=true;
            }
            slot_type=slots.find("Type")->second;
            slot_con=slots.find("Con")->second;
            if(Use_material)
            {
               Doact("explain","Material");
               Doact("request","Menu");
            }
            else if(slot_type!="none")
            {
               Doact("explain","Type",slot_type);
               Doact("request","Menu");
            }
            else if(slot_con!="none")
            {
               Doact("explain","Type",slot_con);
               Doact("request","Menu");
            }
         }
      }
      else if(Request_slot=="Material")
         Doact("menu_example");
      else if(Request_slot=="Type")
         Doact("type_example");
      else if(Request_slot=="Con")
         Doact("con_example");
      //print_act="");
   }
   else if(act=="setting_ok")
   {
      now_act.push(act);
      print_act="알람이 설정되었습니다";
      print_DA=act;
   }
   else if(act=="alarm_end")
   {
      now_act.push(act);
      print_act+=("시간이 되었습니다. 이후 단계를 진행할까요?");
      print_DA+=act;
      output_DA=print_DA;
      output=print_act;
      //SetDlgItemTextW(IDC_EDIT4,print_DA);
      //SetDlgItemTextW(IDC_EDIT3,print_act);
   }
   else if(act=="movie_end")
   {
      now_act.push(act);
      print_act=("재생이 완료되었습니다. 이후 단계를 진행할까요?");
      print_DA=act;
      output_DA=print_DA;
      output=print_act;
      //SetDlgItemTextW(IDC_EDIT4,print_DA);
      //SetDlgItemTextW(IDC_EDIT3,print_act);
   }
   else if(act=="heat_start")
   {
      now_act.push(act);
      print_act=("오븐이 설정되었습니다.");
      print_DA=act;
      output_DA=print_DA;
      output=print_act;
      //SetDlgItemTextW(IDC_EDIT4,print_DA);
      //SetDlgItemTextW(IDC_EDIT3,print_act);
   }
   else if(act=="heat_end")
   {
      now_act.push(act);
      print_act+=("오븐 설정시간이 다 됐습니다. 이후 단계를 진행할까요?");
      print_DA=act;
      output_DA=print_DA;
      output=print_act;
      //SetDlgItemTextW(IDC_EDIT4,print_DA);
      //SetDlgItemTextW(IDC_EDIT3,print_act);
   }
   else if(act=="recipe_end")
   {
      now_act.push(act);
      output_DA=act;
      output="요리가 완료되었습니다. 프로그램을 종료합니다.";
      click_init();
      //SetDlgItemTextW(IDC_EDIT4,act);
      //SetDlgItemTextW(IDC_EDIT3,"요리가 완료되었습니다. 프로그램을 종료합니다.");
   }
   else if(act=="start")
   {
      output_DA="start";
      output="LG Dios 광파오븐 제어기입니다. 무엇을 도와드릴까요?";
      //SetDlgItemTextW(IDC_EDIT4,"start");
      //SetDlgItemTextW(IDC_EDIT3,"LG Dios 광파오븐 제어기 입니다. 무엇을 도와드릴까요?");

      slots.clear();

      for(int i=0;i<55;i++)
         Materials[i]=0;
      for(int i=0;i<25;i++)
      {
         material_point[i]=0;
         con_point[i]=0;
         type_point[i]=0;
      }
      
      //SetDlgItemTextW(IDC_EDIT2,"Goal_label(Material:/ Menu:/ Con:/ Type:), Method_label(), Request_label()");
      Slot_input="";
      now_act.push("start");
      //before_act=TEXT("start");
      ViewNowValue();
   }
   else if(act=="restart")
   {
      //now_act.~Stack();
      
      //slots.clear();
      for(int i=0;i<55;i++)
         Materials[i]=0;
      for(int i=0;i<25;i++)
      {
         material_point[i]=0;
         con_point[i]=0;
         type_point[i]=0;
      }

      //SetDlgItemTextW(IDC_EDIT2,"Goal_label(Material:/ Menu:/ Con:/ Type:), Method_label(), Request_label()");
      Slot_input="";
      ViewNowValue();
      now_act.push("start");
      output_DA="end";
      output="LG Dios 광파오븐 제어기입니다. 무엇을 도와드릴까요?";
	//SetDlgItemTextW(IDC_EDIT4,"start");
      //SetDlgItemTextW(IDC_EDIT3,"LG Dios 광파오븐 제어기 입니다. 무엇을 도와드릴까요?");
   }
   else if(act=="goback")
   {
      print_act="이전으로 이동합니다. ";

      if(now_act.top()=="recipe_q")
      {
         now_act.pop();
      }
      if(now_act.top()=="start")
      {
         print_act+="LG Dios 광파오븐 제어기 입니다. 무엇을 도와드릴까요?";
         output_DA=now_act.top();
         output=print_act;
         //SetDlgItemTextW(IDC_EDIT4,now_act.top());
         //SetDlgItemTextW(IDC_EDIT3,print_act);
      }
      if(now_act.top()=="explain"&&Requested)
      {
         Requested=false;
         Request_slot="";
      }
      else
      {
         if(now_act.top()=="request")
            now_slot.pop();
         now_act.pop();
         if(now_act.top()=="explain")
         {
            string temp_slot=now_slot.top();
            string temp_value=now_value.top();


            if(now_slot.top()=="Menu"||now_slot.top()=="Con"||now_slot.top()=="Type")
            {
               now_slot.pop();
               now_value.pop();
               Doact(now_act.top(),temp_slot,temp_value);
            }
            else if(now_slot.top()=="Material")
            {
               now_slot.pop();
               Doact(now_act.top(),temp_slot);
            }
         }

         if(now_act.top()=="recipe")
         {
               now_slot.pop();
               now_value.pop();
               Doact(now_act.top(),now_slot.top(),now_value.top());
         }
         else if(now_act.top()=="request")
         {
            string temp_slot=now_slot.top();
            now_slot.pop();
            Doact(now_act.top(),temp_slot);
         }
         else
            Doact(now_act.top());
      }
   }
   else if(act=="explain")
   {
      if(now_slot.top()=="material")
      {
         Doact(act,now_slot.top());
         now_slot.pop();
      }
      else
      {
         Doact(act,now_slot.top(),now_value.top());
         now_slot.pop();
         now_value.pop();
      }
   }
   else if(act=="recipe_q")
   {
      now_act.push(act);
      print_act+="그럼 요리법을 안내해 드릴까요?";
      print_DA+="recipe_q";
      output_DA=print_DA;
      output=print_act;
      //SetDlgItemTextW(IDC_EDIT4,print_DA);
      //SetDlgItemTextW(IDC_EDIT3,print_act);
      //SetDlgItemTextW(IDC_EDIT4,now_act.top());
      //SetDlgItemTextW(IDC_EDIT3,print_act);
   }
   else if(act=="start_example")
   {
      now_act.push("start_example");
      print_act="가지고 있는 재료 또는 원하시는 요리명을 말씀해주세요. 요리를 할때 사용하고자 하는 부속품 또는 사용하고자 하는 모드를 말씀해 주세요.";
      output_DA=now_act.top();
      output=print_act;
      //SetDlgItemTextW(IDC_EDIT4,now_act.top());
      //SetDlgItemTextW(IDC_EDIT3,print_act);
   }
   else if(act=="menu_example")
   {
      now_act.push("menu_example");
      print_act="가지고 있는 재료 또는 원하시는 요리명을 말씀해 주세오. 예를 들면 “감자로 만들수 있는 있는 요리는 뭐야? 혹은 “냉동 피자를 만들고 싶습니다” 라고 말씀해 주세요.";
      output_DA=now_act.top();
      output=print_act;
      //SetDlgItemTextW(IDC_EDIT4,now_act.top());
      //SetDlgItemTextW(IDC_EDIT3,print_act);
   }
   else if(act=="con_example")
   {
      now_act.push("con_example");
      print_act="부속품은  법량접시,석쇠,구이전용팬이 있습니다. 예를 들면 “법량접시를 사용하는 요리를 만들고 싶어” 라고 말씀해 주세요.";
      output_DA=now_act.top();
      output=print_act;
      //SetDlgItemTextW(IDC_EDIT4,now_act.top());
      //SetDlgItemTextW(IDC_EDIT3,print_act);
   }
   else if(act=="type_example")
   {
      now_act.push("type_example");
      print_act="모드는 토스트와 오븐이 있습니다. 예를 들면 “토스트 모드를 사용하고 싶어” 라고 말씀해 주세요.";
      output_DA=now_act.top();
      output=print_act;
      //SetDlgItemTextW(IDC_EDIT4,now_act.top());
      //SetDlgItemTextW(IDC_EDIT3,print_act);
   }
   else if(act=="null")
   {
      if(now_act.top()=="start")
         Doact("start_example");
      else if(now_act.top()=="start_example")
         Doact("menu_example");
      else if(now_act.top()=="menu_example")
         Doact("con_example");
      else if(now_act.top()=="con_example")
         Doact("type_example");
      else if(now_act.top()=="explain")
      {
         Doact("plase_repeat");
      }
      else
      {
         print_act="죄송합니다. 잘 모르겠습니다. 이후 단계를 진행하시겠습니까?";
        output_DA="sorry";
        output=print_act;
         // SetDlgItemTextW(IDC_EDIT4,"sorry");
        // SetDlgItemTextW(IDC_EDIT3,print_act);
      }
   }
   else if(act=="repeat")
   {
      if(now_act.top()=="recipe_q")
         now_act.pop();
      
      if(now_act.top()=="recipe")
         Doact("recipe",now_slot.top(),now_value.top());
      else if(now_act.top()=="explain")
      {
         if(now_slot.top()=="Type"||now_slot.top()=="Con")
            Doact("explain",now_slot.top(),now_value.top());
         else if(now_slot.top()=="Menu")
         {
            Doact("explain",now_slot.top(),now_value.top());
            Doact("recipe_q");
         }
         else 
         {
            Doact("explain",now_slot.top());
         }
      }
      else
         Doact(now_act.top());
   }
   else if(act=="affirm")
   {
      string slot_menu;
      if(now_act.top()=="start")
      {
         print_act="잘못된 입력입니다. 다시 한번 말씀해 주십시오.";
         output_DA="please_repeat";
         output=print_act;
         //SetDlgItemTextW(IDC_EDIT4,"please_repeat");
         //SetDlgItemTextW(IDC_EDIT3,print_act);
      }
      else if(now_act.top()=="recipe_q")
      {
         string slot_menu;
         slot_menu=slots.find("Menu")->second;
        // slots.Lookup("Menu",slot_menu);
         Doact("recipe",slot_menu,"1");
         //print_act=
         //SetDlgItemTextW(IDC_EDIT4,"please_repeat"));
         //SetDlgItemTextW(IDC_EDIT3,print_act);
         //impl_confirm("Menu",slot_menu);
      }
      else if(now_act.top()=="recipe"||now_act.top()=="alarm_end"||now_act.top()=="movie_end"||now_act.top()=="heat_end")
      {
         bool Done=false;
         string Video_menu[5]={"gratin","roll_cake","scorn","sponge_cake","chocochip"};
         int Video_phase[5]={2,1,1,1,1};
         string Heat_menu[25]={"gratin","roasted_sweet_potato","frozen_pizza","nurungji","drumstick","Tortilla","roll_cake",
            "barbeque","scorn","sponge_cake","sweet_rice","deluxe_rice","spaghetti","chocochip","cheese_cake",
            "potato","chicken","pork_belly","pizza","hamburger","garlic_bread","bagle","bread","pizza_toast",
            "honey_bread"};
         int Heat_phase[25]={6,2,1,2,2,5,11,3,6,9,3,5,6,7,5,2,3,2,7,4,2,2,1,7,4};
         string alarm_menu[10]={"drumstick","Tortilla","barbeque","scorn","sweet_rice","sweet_rice","deluxe_rice",
            "deluxe_rice","chicken","pizza_toast"};
         int alarm_phase[10]={1,1,1,3,1,4,1,6,1,1};

         for(int i=0;i<5;i++)
         {
            if(now_slot.top()==Video_menu[i]&&atoi(now_value.top().c_str())==Video_phase[i])
            {
               Doact("movie_end");
               Done=true;
            }
         }
         for(int i=0;i<25;i++)
         {
            if(now_slot.top()==Heat_menu[i]&&atoi(now_value.top().c_str())==Heat_phase[i])
            {
               Doact("heat_start");
               Doact("heat_end");
               Done=true;
            }
         }
         for(int i=0;i<10;i++)
         {
            if(now_slot.top()==alarm_menu[i]&&atoi(now_value.top().c_str())==alarm_phase[i])
            {
               Doact("setting_ok");
               Doact("alarm_end");
               Done=true;
            }
         }
         if(!Done)
         {
            string str;
            stringstream ss;
            int num;
            num=atoi(now_value.top().c_str())+1;
            ss<<num;
            ss>>str;
            //str.Format( "%d",(stoi(now_value.top())+1));
            Doact("recipe",now_slot.top(),str);
         }
      }
      else
      {
         Doact("please_repeat");
      }
   }
   else if(act=="negate")
   {
      if(now_act.top()=="start")
      {
         print_act="잘못된 입력입니다. 다시 한번 말씀해 주십시오.";
        output_DA="please_repeat";
        output=print_act;
         //SetDlgItemTextW(IDC_EDIT4,"please_repeat");
         //SetDlgItemTextW(IDC_EDIT3,print_act);
      }
      else if(now_act.top()=="recipe_q")
      {
         Doact("end");
      }
   }
   else if(act=="alarm_start")
   {
      bool Done=false;
      if(now_act.top()=="recipe")
      {
         string alarm_menu[10]={"drumstick","Tortilla","barbeque","scorn","sweet_rice","sweet_rice","deluxe_rice",
            "deluxe_rice","chicken","pizza_toast"};
         int alarm_phase[10]={1,1,1,3,1,4,1,6,1,1};

         for(int i=0;i<10;i++)
         {
            if(now_slot.top()==alarm_menu[i]&&atoi(now_value.top().c_str())==alarm_phase[i])
            {
               Doact("setting_ok");
               Doact("alarm_end");
               Done=true;
            }
         }
         if(!Done)
            Doact("sorry");
      }
      else
         Doact("please_repeat");
      
   }
   else if(act=="movie_start")
   {
      bool Done=false;
      if(now_act.top()=="recipe")
      {
         string Video_menu[5]={"gratin","roll_cake","scorn","sponge_cake","chocochip"};
         int Video_phase[5]={2,1,1,1,1};

         for(int i=0;i<5;i++)
         {
            if(now_slot.top()==Video_menu[i]&&atoi(now_value.top().c_str())==Video_phase[i])
            {
               Doact("movie_end");
               Done=true;
            }
         }
         if(!Done)
            Doact("sorry");
      }
      else
         Doact("please_repeat");
   }
   else if(act=="heat_start")
   {
      bool Done=false;
      if(now_act.top()=="recipe")
      {
         string Heat_menu[25]={"gratin","roasted_sweet_potato","frozen_pizza","nurungji","drumstick","Tortilla","roll_cake",
            "barbeque","scorn","sponge_cake","sweet_rice","deluxe_rice","spaghetti","chocochip","cheese_cake",
            "potato","chicken","pork_belly","pizza","hamburger","garlic_bread","bagle","bread","pizza_toast",
            "honey_bread"};
         int Heat_phase[25]={6,2,1,2,2,5,11,3,6,9,3,5,6,7,5,2,3,2,7,4,2,2,1,7,4};

         for(int i=0;i<5;i++)
         {
            if((now_slot.top()==Heat_menu[i])&&(atoi(now_value.top().c_str())==Heat_phase[i]))
            {
               Doact("heat_start");
               Doact("heat_end");
               Done=true;
            }
         }
         if(!Done)
            Doact("sorry");
      }
      else
         Doact("please_repeat");
   }
   else if(act=="next")
   {
      if(now_act.top()=="start")
      {
         print_act="잘못된 입력입니다. 다시 한번 말씀해 주십시오.";
         output_DA="please_repeat";
         output=print_act;
         //SetDlgItemTextW(IDC_EDIT4,"please_repeat");
         //SetDlgItemTextW(IDC_EDIT3,print_act);
      }
      else if(now_act.top()=="recipe_q")
      {
         string slot_menu;
         slot_menu=slots.find("Menu")->second;
         //slots.Lookup("Menu",slot_menu);
         Doact("recipe",slot_menu,"1");
         //print_act=
         //SetDlgItemTextW(IDC_EDIT4,TEXT("please_repeat"));
         //SetDlgItemTextW(IDC_EDIT3,print_act);
         //impl_confirm("Menu",slot_menu);
      }
      else if(now_act.top()=="recipe")
      {
         bool Done=false;
         string Heat_menu[25]={"gratin","roasted_sweet_potato","frozen_pizza","nurungji","drumstick","Tortilla","roll_cake",
            "barbeque","scorn","sponge_cake","sweet_rice","deluxe_rice","spaghetti","chocochip","cheese_cake",
            "potato","chicken","pork_belly","pizza","hamburger","garlic_bread","bagle","bread","pizza_toast",
            "honey_bread"};
         int Heat_phase[25]={6,2,1,2,2,5,11,3,6,9,3,5,6,7,5,2,3,2,7,4,2,2,1,7,4};
         for(int i=0;i<25;i++)
         {
            if(now_slot.top()==Heat_menu[i]&&atoi(now_value.top().c_str())==Heat_phase[i])
            {
               Doact("heat_start");
               Doact("heat_end");
               Done=true;
            }
         }
         //CString slot_menu;
         //slots.Lookup(TEXT("Menu",slot_menu);
         if(!Done)
         {
            string str;
            int num;
            stringstream ss;

            num=(atoi(now_value.top().c_str())+1);
            ss<<num;
            ss>>str;
            //str=std::to_string(num);
            Doact("recipe",now_slot.top(),str);
         }
         //int endlevel[25]={6,2,1,2,3,5,13,};
      }
      else if(now_act.top()=="heat_end"||now_act.top()=="alarm_end"||now_act.top()=="video_end")
      {
            string str;
            int num;
            stringstream ss;

            num=(atoi(now_value.top().c_str())+1);
            ss<<num;
            ss>>str;
            //str=std::to_string(num);
            Doact("recipe",now_slot.top(),str);
      }
      else
      {
         Doact("please_repeat");
      }
   }
   else if(act=="please_repeat")
   {
      print_act="잘못된 입력입니다. 다시 한번 말씀해 주십시오.";
      output_DA="please_repeat";
      output=print_act;
      //SetDlgItemTextW(IDC_EDIT4,"please_repeat");
      //SetDlgItemTextW(IDC_EDIT3,print_act);
   }
   else if(act=="reqalts")
   {
      if(now_act.top()=="start")
      {
         print_act="잘못된 입력입니다. 다시 한번 말씀해 주십시오.";
         output_DA="please_repeat";
         output=print_act;
         //SetDlgItemTextW(IDC_EDIT4,"please_repeat");
         //SetDlgItemTextW(IDC_EDIT3,print_act);
      }
   }
}
string CTab3::Find_menu()
{
   string Menu_Names[25]={"감자 그라탱","군고구마","냉동피자","누룽지",
      "닭다리","또띠아 피자","롤케이크","립 바비큐",
      "스콘","스폰지 케이크","약식","영양밥","오븐 스파게티",
      "초코칩 쿠키","치즈 케이크","통감자","통닭","통삼겹살","피자",
      "햄버그 스테이크","마늘빵","베이글","식빵","피자토스트","허니브레드"};
   
   string list;
   bool first=true;
   int highst=0;

   get_bycon();
   get_bymaterial();
   get_bytype();
   for(int i=0;i<25;i++)
   {
      if(highst<type_point[i]+con_point[i]+material_point[i])
         highst=type_point[i]+con_point[i]+material_point[i];
   }

   for(int i=0;i<25;i++)
   {
      if(highst==type_point[i]+con_point[i]+material_point[i])
      {
         if(first)
         {
            list=Menu_Names[i];
            first=false;
         }
         else
            list+=", "+Menu_Names[i];
      }
   }
   return list;
}
string CTab3::Find_Material(string language)
{
   string Material_Names[55]={"가지","감자","계피가루","고구마","꿀",
      "냉동피자","다진 돼지고기","다진마늘","다진쇠고기","다진실파",
      "달걀","닭다리","대추","돼지고기 립","또띠아","멥쌀",
      "모짜렐라 치즈","바닐라오일","박력분","밤","버터","베이글",
      "베이컨","베이킹 소다","베이킹파우더","블랙올리브","생닭",
      "생크림","스파게티면","식빵","식은밥","양송이","양파",
      "오레가노","올리고당","우스터 소스","월계수잎","육수","은행",
      "잣","적포도주","중력분","찹쌀","청피망","초코칩","콩",
      "크림치즈","토마토 페이스트","통삼겹살","파마산 치즈","파슬리 가루",
      "피자 빵가루","피자소스","햄","홍피망"};
   string Material_ENames[55]={"eggplant","potato","cinnamon_powder","sweet_potato",
      "honey","pizza","pork","crushed_galic","beef","small_green_onion",
      "egg","drumstick","jujube","pork_rib","Tortilla","nonglutinous_rice",
      "cheese","vanilla_oil","soft_flour","chestnut","butter","bagle",
      "bacon","baking_soda","baking_powder","black_olive","chicken",
      "cream","spaghetti","bread","rice","button_mushroom","onion",
      "oregano","oligosaccharides","worcester_sauce","bay_leaf","meat_broth",
      "ginkgo_nut","pine_nut","red_wine","medium_flour","sticky_rice",
      "squid","chocochip","bean","cream_cheese","tomato_paste",
      "pork_belly","parmesan","parsley","bread_crumbs","pizza_source",
      "ham","red_peppers"};

   string list;
   bool first=true;

   for(int i=0;i<55;i++)
   {
      if(Materials[i]==1)
      {
         if(first)
         {
            if(language=="korean")
               list=Material_Names[i];
            else if(language=="english")
               list=Material_ENames[i];

            first=false;
         }
         else
         {

            if(language=="korean")
               list+=", "+Material_Names[i];
            else if(language=="english")
               list+=", "+Material_ENames[i];
         }
      }
   }
   return list;
}
void CTab3::get_bytype()
{
   string slot_type;
   slot_type=slots.find("Type")->second;
   if(slot_type!="none")
   {
      if(slot_type=="oven")
      {
         for(int i=0; i<20;i++)
            type_point[i]=100;
         for(int i=20; i<25;i++)
            type_point[i]=-300;
      }
      else if(slot_type=="toast")
      {
         for(int i=0; i<20;i++)
            type_point[i]=-300;
         for(int i=20; i<25;i++)
            type_point[i]=100;
      }
   }
}
void CTab3::get_bycon()
{
   string slot_con;
   slot_con=slots.find("Con")->second;
   if(slot_con!="none")
   {
      if(slot_con=="plate")
      {
         for(int i=0; i<3;i++)
            con_point[i]=100;
         for(int i=3; i<6;i++)
            con_point[i]=-300;
         for(int i=6; i<18;i++)
            con_point[i]=100;
         for(int i=18; i<20;i++)
            con_point[i]=-300;

         con_point[20]=100;//마늘빵
         con_point[21]=-300;
         con_point[22]=-300;
         con_point[23]=100;//피자토스트
         con_point[24]=100;//허니브레드
      }
      else if(slot_con=="grate")
      {
         for(int i=0; i<3;i++)
            con_point[i]=100;
         for(int i=3; i<7;i++)
            con_point[i]=-300;

         con_point[7]=100;
         con_point[8]=-300;
         for(int i=9; i<13;i++)
            con_point[i]=100;
         con_point[13]=-300;
         for(int i=14; i<18;i++)
            con_point[i]=100;
         for(int i=18; i<25;i++)
            con_point[i]=-300;
      }
      else if(slot_con=="pan")
      {
         for(int i=0; i<3;i++)
            con_point[i]=-300;
         for(int i=3; i<6;i++)
            con_point[i]=100;
         for(int i=6; i<13;i++)
            con_point[i]=-300;

         con_point[13]=100;

         for(int i=14; i<18;i++)
            con_point[i]=-300;
         con_point[18]=100;
         con_point[19]=100;
         con_point[20]=-300;
         con_point[21]=100;
         con_point[22]=100;
         con_point[23]=-300;
         con_point[24]=-300;
      }
   }
}
void CTab3::get_bymaterial()
{
   for(int i=0;i<55;i++)
   {
      if(Materials[i]==1)
      {
         switch(i)
         {
#pragma region materials
         case 0://가지
            material_point[12]+=10;
            break;
         case 1://감자
            material_point[0]+=10;
            material_point[15]+=10;
            break;
         case 2://계피가루
            material_point[10]+=10;
            break;
         case 3://고구마
            material_point[1]+=10;
            break;
         case 4://꿀
            material_point[24]+=10;
            break;
         case 5://냉동피자
            material_point[2]+=10;
            break;
         case 6://다진 돼지고기
            material_point[19]+=10;
            break;
         case 7://다진 마늘
            material_point[18]+=10;
            material_point[19]+=10;
            material_point[20]+=10;
            break;
         case 8://다진 쇠고기
            material_point[18]+=10;
            material_point[19]+=10;
            break;
         case 9://다진 실파
            material_point[12]+=10;
            break;
         case 10://달걀
            material_point[6]+=10;
            material_point[8]+=10;
            material_point[9]+=10;
            material_point[13]+=10;
            material_point[14]+=10;
            material_point[19]+=10;
            break;
         case 11://닭다리
            material_point[4]+=10;
            break;
         case 12://대추
            material_point[10]+=10;
            material_point[11]+=10;
            break;
         case 13://돼지고기 립
            material_point[7]+=10;
            break;
         case 14://또띠아
            material_point[5]+=10;
            break;
         case 15://멥쌀
            material_point[11]+=10;
            break;
         case 16://모짜렐라 치즈
            material_point[0]+=10;
            material_point[5]+=10;
            material_point[12]+=10;
            material_point[18]+=10;
            material_point[23]+=10;
            break;
         case 17://바닐라 오일
            material_point[6]+=10;
            material_point[9]+=10;
            break;
         case 18://박력분
            material_point[6]+=10;
            material_point[9]+=10;
            material_point[13]+=10;
            material_point[14]+=10;
            break;
         case 19://밤
            material_point[10]+=10;
            material_point[11]+=10;
            break;
         case 20://버터
            material_point[20]+=10;
            material_point[23]+=10;
            material_point[24]+=10;
         case 21://베이글
            material_point[21]+=10;
            break;
         case 22://베이컨
            material_point[0]+=10;
            material_point[5]+=10;
            material_point[18]+=10;
            material_point[23]+=10;
            break;
         case 23://베이킹소다
            material_point[13]+=10;
            break;
         case 24://베이킹파우더
            material_point[8]+=10;
            break;
         case 25://블랙올리브
            material_point[5]+=10;
            material_point[18]+=10;
            material_point[23]+=10;
            break;
         case 26://생닭
            material_point[16]+=10;
            break;
         case 27://생크림
            material_point[14]+=10;
            break;
         case 28://스파게티면
            material_point[12]+=10;
            break;
         case 29://식빵
            material_point[20]+=10;
            material_point[22]+=10;
            material_point[23]+=10;
            material_point[24]+=10;
            break;
         case 30://식은밥
            material_point[3]+=10;
            break;
         case 31://양송이
            material_point[5]+=10;
            material_point[18]+=10;
            material_point[23]+=10;
            break;
         case 32://양파
            material_point[0]+=10;
            material_point[5]+=10;
            material_point[18]+=10;
            material_point[19]+=10;
            material_point[23]+=10;
            break;
         case 33://오레가노
            material_point[18]+=10;
            break;
         case 34://올리고당
            //material_point[24]+=10;
            break;
         case 35://우스터소스
            material_point[19]+=10;
         case 36://월계수잎
            material_point[18]+=10;
            break;
         case 37://육수
            material_point[18]+=10;
            material_point[19]+=10;
            break;
         case 38://은행
            material_point[11]+=10;
            break;
         case 39://잣
            material_point[10]+=10;
            material_point[11]+=10;
            break;
         case 40://적포도주
            material_point[19]+=10;
            break;
         case 41://중력분
            material_point[8]+=10;
            break;
         case 42://찹쌀
            material_point[10]+=10;
            material_point[11]+=10;
            break;
         case 43://청피망
            material_point[5]+=10;
            material_point[18]+=10;
            material_point[23]+=10;
            break;
         case 44://초코칩
            material_point[13]+=10;
            break;
         case 45://콩
            material_point[11]+=10;
            break;
         case 46://크림치즈
            material_point[13]+=10;
            break;
         case 47://토마토 페이스트
            material_point[18]+=10;
            break;
         case 48://통삽겹살
            material_point[17]+=10;
            break;
         case 49://파마산치즈
            material_point[12]+=10;
            break;
         case 50://파슬리 가루
            material_point[20]+=10;
         case 51://피자 빵가루
            material_point[18]+=10;
            material_point[19]+=10;
            break;
         case 52://피자소스
            material_point[5]+=10;
            break;
         case 53://햄
            material_point[5]+=10;
            material_point[18]+=10;
            material_point[23]+=10;
            break;
         case 54://홍피망
            material_point[5]+=10;
            material_point[18]+=10;
            material_point[23]+=10;
            break;
         default:
            break;
#pragma endregion add_point
         }
      }
   }
}
string CTab3::Menu_Material(string menu)
{
   string list;
   string Menu_Names[25]={"감자 그라탱","군고구마","냉동피자","누룽지",
      "닭다리","또띠아 피자","롤케이크","립 바비큐",
      "스콘","스폰지 케이크","약식","영양밥","오븐 스파게티",
      "초코칩 쿠키","치즈 케이크","통감자","통닭","통삼겹살","피자",
      "햄버그 스테이크","마늘빵","베이글","식빵","피자토스트","허니브레드"};
   int index=0;

   for(int i=0;i<25;i++)
   {
      if(menu==Menu_Names[i])
         index=i;
   }

   switch(index)
   {
   case 0:
      list="감자, 베이컨, 양파, 모짜렐라 치즈";
      break;
   case 1:
      list="고구마";
      break;
   case 2:
      list="냉동피자";
      break;
   case 3:
      list="식은밥";
      break;
   case 4:
      list="닭다리";
      break;
   case 5:
      list="8인치 또띠아, 시판하는 피자소스, 베이컨, 햄, 양송이, 청피망, 홍피망, 양파, 블랙 올리브, 모짜렐라 치즈";
      break;
   case 6:
      list="박력분, 달걀, 바닐라오일";
      break;
   case 7:
      list="돼지고기 립";
      break;
   case 8:
      list="중력분, 베이킹파우더, 달걀";
      break;
   case 9:
      list="박력분, 바닐라 오일, 달걀";
      break;
   case 10:
      list="찹쌀, 계피가루, 밤, 대추, 잣";
      break;
   case 11:
      list="멥쌀, 찹쌀, 밤, 대추, 잣, 콩, 은행";
      break;
   case 12:
      list="스파게티면, 가지, 모짜렐라 치즈, 다진 실파, 파마산 치즈";
      break;
   case 13:
      list="박력분, 베이킹파우더, 베이킹 소다, 달걀, 초코칩";
      break;
   case 14:
      list="크림치즈, 달걀, 박력분, 생크림";
      break;
   case 15:
      list="감자";
      break;
   case 16:
      list="생닭";
      break;
   case 17:
      list="통삼겹살";
      break;
   case 18:
      list="피자 빵가루, 다진마늘, 다진양파, 다진 쇠고기, 토마토 페이스트, 월계수잎, 오레가노, 육수, 베이컨, 햄, 양송이, 청피망, 홍피망, 양파, 블랙올리브, 모짜렐라 치즈";
      break;
   case 19:
      list="다진 쇠고기, 다진 돼지고기, 다진 마늘, 다진 양파, 달걀, 빵가루, 양파, 우스터 소스, 적포도주, 육수";
      break;
   case 20:
      list="식빵, 다진 마늘, 다진 파슬리, 버터";
      break;
   case 21:
      list="베이글";
      break;
   case 22:
      list="식빵";
      break;
   case 23:
      list="식빵, 버터, 피자 소스, 베이컨, 햄, 양송이, 청피망, 홍피망, 양파, 블랙올리브, 모짜렐라";
      break;
   case 24:
      list="식빵, 꿀, 버터";
      break;
   }
   return list;
}
string CTab3::Menu_Con(string con)
{
   if(con=="plate")
      return "감자 그라탱, 군고구마, 냉동피자, 롤케이크, 립 바비큐, 스콘, 스폰지 케이크, 약식, 영양밥, 오븐 스파게티, 초코칩 쿠키, 치즈 케이크, 통감자, 통닭, 통삼겹살, 마늘빵, 피자토스트, 허니브레드";
   else if(con=="grate")
      return "감자 그라탱, 군고구마, 냉동피자, 립 바비큐, 스폰지 케이크, 약식, 영양밥, 오븐 스파게티, 치즈 케이크, 통감자, 통닭, 통삼겹살";
   else if(con=="pan")
      return "누룽지, 닭다리, 또띠아 피자, 초코칩 쿠키, 피자, 햄버그 스테이크, 베이글, 식빵";
}
string CTab3::Menu_Type(string type)
{
   if(type=="oven")
      return "감자 그라탱, 군고구마, 냉동피자, 누룽지, 닭다리, 또띠아 피자, 롤케이크, 립 바비큐, 스콘, 스폰지 케이크, 약식, 영양밥, 오븐 스파게티, 초코칩 쿠키, 치즈 케이크, 통감자, 통닭, 통삼겹살, 피자, 햄버그 스테이크";
   else if(type=="toast")
      return "마늘빵, 베이글, 식빵, 피자토스트, 허니브레드";
}
string CTab3::Conv_Korean(string ename)
{
   string Menu_Names[30]={"오븐","토스트","법량접시","석쇠","구이전용팬","감자 그라탱","군고구마","냉동피자","누룽지",
      "닭다리","또띠아 피자","롤케이크","립 바비큐",
      "스콘","스폰지 케이크","약식","영양밥","오븐 스파게티",
      "초코칩 쿠키","치즈 케이크","통감자","통닭","통삼겹살","피자",
      "햄버그 스테이크","마늘빵","베이글","식빵","피자토스트","허니브레드"};
   string Menu_ENames[30]={"oven","toast","plate","grate","pan","gratin","roasted_sweet_potato","frozen_pizza","nurungji","drumstick",
      "Tortilla","roll_cake","barbeque","scorn","sponge_cake","sweet_rice","deluxe_rice",
      "spaghetti","chocochip","cheese_cake","potato","chicken","pork_belly","pizza",
      "hamburger","garlic_bread","bagle","bread","pizza_toast","honey_bread"};

   for(int i=0;i<30;i++)
   {
      if(ename==Menu_ENames[i])
         return Menu_Names[i];
   }
}
string CTab3::Find_recipe(string menu,int level)
{
   if(menu=="gratin")
   {
      switch(level)
      {
      case 1:
         return "감자는 삶아 으깨어서 준비합니다. 다음단계로 진행할까요?";
         break;
      case 2:
         return "베이컨, 양파를 채썰어 버터에 볶아둡니다. 채썰기 영상을 보시겠습니까?";
         break;
      case 3:
         return "팬에 밀가루, 버터를 넣어 볶은 후 우유를 조금씩 넣어 가며 밀가루 멍울을 풀고 소금, 후추로 간하여 소스를 만듭니다. 이때 간을 약간 세게 하고 소스는 약간 묽게 합니다. 다음단계로 진행할까요?";
         break;
      case 4:
         return "화이트소스 이분의 삼 분량에 볶은 재료와 감자를 넣어 섞습니다. 다음단계로 진행할까요?";
         break;
      case 5:
         return "그라탱접시, 지름 23센티 정도의 내열접시에 소스를 버무린 재료를 담고 남은 소스를 끼얹은 후 다진 치즈, 빵가루, 파슬리 순으로 올립니다. 다음단계로 진행할까요?";
         break;
      case 6:
         return "법랑접시에 석쇠를 놓고 그라탱 접시를 올린 후 2단에 넣어주세요. 준비되시면 다음이라고 말씀해주세요.";
         break;
      default:
         return "end";
         break;
      }
   }
   else if(menu=="roasted_sweet_potato")
   {
      switch(level)
      {
      case 1:
         return "고구마는 너무 크지 않는 것으로 골라 깨끗이 씻은 후 물기를 닦아 껍질을 포크로 몇차례 찔러 줍니다. 다음단계로 진행할까요?";
         break;
      case 2:
         return "1단에 법랑접시 위에 석쇠를 놓고 고구마를 가지런히 올려주세요. 준비되시면 다음이라고 말씀해주세요.";
         break;
      default:
         return "end";
         break;
      }
   }
   else if(menu=="frozen_pizza")
   {
      switch(level)
      {
      case 1:
         return "법랑접시를 석쇠에 놓고 포장을 제거한 냉동피자를 올린 후 3단에 넣어주세요. 준비되시면 다음이라고 말씀해주세요.";
         break;
      default:
         return "end";
         break;
      }
   }
   else if(menu=="nurungji")
   {
      switch(level)
      {
      case 1:
         return "상온에 보관된 식은 밥 이백그람을 구이전용 팬 위에 얇게 전체로 펴 줍니다. 다음단계로 진행할까요?";
         break;
      case 2:
         return "자동요리오븐에서 누룽지를 선택한 후 4단에 올려주세요. 준비되시면 다음이라고 말씀해주세요.";
         break;
      default:
         return "end";
         break;
      }
   }
   else if(menu=="drumstick")
   {
      switch(level)
      {
      case 1:
         return "닭다리에 칼집을 넣어준 후 소금, 후추, 청주에 약 삼십분 정도 재웁니다. 이를 위한 알람 셋팅을 하시겠습니까?";
         break;
      case 2:
         return "닭다리에 올리브유를 바른 후 구이전용 팬 위에 가지런히 놓고, 4단에 넣어주세요. 준비되시면 다음이라고 말씀해주세요.";
         break;
      case 3:
         return "도중에 멜로디가 울리면 한번 뒤집어줍니다. 다음단계로 진행할까요?";
         break;
      default:
         return "end";
         break;
      }
   }
   else if(menu=="Tortilla")
   {
      switch(level)
      {
      case 1:
         return "토핑을 다음과 같이 준비합니다. 먼저 양파, 청, 홍피망, 양송이는 적당한 크기롤 썰어서 내열용기에 담고 랩을 씌워 수동요리레인지 기능으로 2~3분 가열한 후 물기를 없앱니다.  알람설정을 하시겠습니까?";
         break;
      case 2:
         return "베이컨, 햄은 적당한 크기로 자릅니다. 모짜렐라 치즈는 잘게 다지고, 올리브는 모양대로 얇게 썰어줍니다. 다음단계로 진행할까요?";
         break;
      case 3:
         return "또띠아 2장을 겹쳐 구이전용 팬에 올립니다. 다음단계로 진행할까요?";
         break;
      case 4:
         return "가운데 부분에 피자소스를 고루 펴 바르고 햄, 베이컨, 양송이, 양파, 피망 순으로 토핑합니다. 다음단계로 진행할까요?";
         break;
      case 5:
         return "모짜렐라 치즈를 고루 뿌린 뒤 올리브로 장식하여 4단에 넣어주세요. 준비되시면 다음이라고 말씀해주세요.";
         break;
      default:
         return "end";
         break;
      }
   }
   else if(menu=="roll_cake")
   {
      switch(level)
      {
      case 1:
         return "박력분은 체쳐 둡니다. 체치기 영상을 보시겠습니까?";
         break;
      case 2:
         return "설탕은 적당히 반으로 나눕니다. 다음단계로 진행할까요?";
         break;
      case 3:
         return "볼을 2개 준비하여 한쪽은 흰자, 다른 한 쪽은 노른자를 분리해 놓고 노른자 1개는 따로 빼둡니다. 다음단계로 진행할까요?";
         break;
      case 4:
         return "노른자를 풀어 소금, 설탕, 물엿, 우유를 함께 넣고 아이보리색이 되도록 거품을 냅니다. 다음단계로 진행할까요?";
         break;
      case 5:
         return "흰자는 어느 정도 거품을 올린 후 설탕을 조금씩 나누어 넣으며 거품을 빳빳하게 내줍니다. 다음단계로 진행할까요?";
         break;
      case 6:
         return "노른자에 흰자를 삼분의 일정도 넣고 가볍게 섞다가 체에 친 밀가루를 넣어 주걱으로 가볍고 빠르게 섞어줍니다. 다음단계로 진행할까요?";
         break;
      case 7:
         return "식용유를 넣어 고루 섞은 다음 나머지 흰자 거품을 넣어 거품이 가라앉지 않도록 재빨리 섞어 줍니다. 다음단계로 진행할까요?";
         break;
      case 8:
         return "법랑접시에 유산지를 깔고 반죽을 평평하게 부어줍니다. 다음단계로 진행할까요?";
         break;
      case 9:
         return "노른자 1개에 소금을 약간 넣어 잘 풀어 비닐봉지에 담고 구멍을 작게 뚫어 반죽 위에 흘리면서 지그재그로 무늬를 만들어 줍니다. 다음단계로 진행할까요?";
         break;
      case 10:
         return "무늬 반대편을 젓가락으로 다시 지그재그로 휘저어 물결 무늬를 만듭니다. 다음단계로 진행할까요?";
         break;
      case 11:
         return "오븐 2단에 재료를 넣어주세요. 준비되시면 다음이라고 말씀해주세요.";
         break;
      case 12:
         return "젖은 면보를 준비하여 바닥에 깔고 구워낸 케이크를 바닥이 위로 오도록 엎어 줍니다. 다음단계로 진행할까요?";
         break;
      case 13:
         return "케이크 위에 잼을 고루 바른 뒤 면보 아래로 방망이를 대고 케이크를 밀어 가며 말아줍니다. 다음단계로 진행할까요?";
         break;
      default:
         return "end";
         break;
      }
   }
   else if(menu=="barbeque")
   {
      switch(level)
      {
      case 1:
         return "립은 통으로 준비하여 기름을 떼네고 찬물에 2시간정도 담가 핏물을 뺍니다. 이를 위한 알람셋팅을 하시겠습니까?";
         break;
      case 2:
         return "물기를 제거하고 레몬 즙, 소금, 후추로 밑간을 합니다. 다음단계로 진행할까요?";
         break;
      case 3:
         return "법랑접시 위에 석쇠를 놓고 립을 올려주세요. 준비되시면 다음이라고 말씀해주세요.";
         break;
      case 4:
         return "분량대로 섞어 소스를 만듭니다. 다음단계로 진행할까요?";
         break;
      case 5:
         return "도중에 멜로디가 울리면 소스를 붓으로 고루 바른 후 남은 시간 동안 더 구워줍니다. 다음단계로 진행할까요?";
         break;
      case 6:
         return "립을 접시에 담고 소스를 한번 더 끼얹어 냅니다. 다음단계로 진행할까요?";
         break;
      default:
         return "end";
         break;
      }
   }
   else if(menu=="scorn")
   {
      switch(level)
      {
      case 1:
         return "중력분, 베이킹 파우더는 체치고 설탕, 소금, 냉장 보관한 버터를 넣고 자르듯이 섞습니다. 버터와 밀가루가 고루 섞여 부슬부슬한 상태가 되도록 만듭니다. 이에 관한 체치기 동영상을 보시겠습니까?";
         break;
      case 2:
         return "미리 섞은 달걀과 우유를 1에 넣고 자르듯 고루 섞습니다. 다음단계로 진행할까요?";
         break;
      case 3:
         return "마른 가루가 보이지 않고, 반죽이 하나로 뭉쳐지면 랩을 덮어 냉장고에서 30분정도 숙성 시킵니다. 이를 위한 알림셋팅을 하시겠습니까?";
         break;
      case 4:
         return "반죽을 약 2.5센치 두께가 되도록 밀대로 민 다음 같은 크기로 자릅니다. 이때 덧가루를 사용해야 밀어 펴기가 편리합니다. 다음단계로 진행할까요?";
         break;
      case 5:
         return "법랑 접시에 유산지를 깔고 스콘을 올린 후 표면에 달걀물을 칠합니다. 다음단계로 진행할까요?";
         break;
      case 6:
         return "2단에 해당 법랑접시를 넣어주세요. 준비되시면 다음이라고 말씀해주세요.";
         break;
      default:
         return "end";
         break;
      }
   }
   else if(menu=="sponge_cake")
   {
      switch(level)
      {
      case 1:
         return "박력분은 체쳐 둡니다. 체치기 영상을 보시겠습니까?";
         break;
      case 2:
         return "케이크 틀 바닥과 옆면에 종이를 깔아 줍니다. 다음단계로 진행할까요?";
         break;
      case 3:
         return "버터를 우유에 함께 녹여 준 후 바닐라 오일을 한 방울 섞어 줍니다. 다음단계로 진행할까요?";
         break;
      case 4:
         return "달걀을 따뜻한 물에서 중탕한 후 볼에 넣고 풀어줍니다. 다음단계로 진행할까요?";
         break;
      case 5:
         return "달걀과 설탕을 한꺼번에 넣고 거품기를 들어보아 끝이 뾰족하고 살짝 숙여질 때까지 거품을 냅니다. 다음단계로 진행할까요?";
         break;
      case 6:
         return "거품낸 달걀에 박력분을 넣고 골고루 섞어 줍니다. 다음단계로 진행할까요?";
         break;
      case 7:
         return "위 반죽의 일부를 버터 녹인 우유에 넣어 섞은 후 다시 반죽에 넣어 재빨리 섞어 줍니다. 다음단계로 진행할까요?";
         break;
      case 8:
         return "반죽을 틀에 붓고 틀을 한번 들었다가 바닥에 놓아 반죽 속의 기포를 고르게 해 줍니다. 다음단계로 진행할까요?";
         break;
      case 9:
         return "법랑접시 위에 석쇠를 놓고 케이크 틀을 올린 후 1단에 넣어주세요. 준비되시면 다음이라고 말씀해주세요.";
         break;
      default:
         return "end";
         break;
      }
   }
   else if(menu=="sweet_rice")
   {
      switch(level)
      {
      case 1:
         return "찹쌀을 한 시간 이상 물에 불려 체에 건져 둡니다. 이를 위한 알람 셋팅을 하시겠습니까?";
         break;
      case 2:
         return "밤은 껍질을 벗겨 4등분하고, 대추는 씨를 빼내어 2등분하고, 잣은 고깔을 떼어냅니다. 다음단계로 진행할까요?";
         break;
      case 3:
         return "법랑 접시에 석쇠를 놓고 2리터 정도 크기의 내열 용기를 올린 후 뚜껑 또는 알루미늄 호일로 덮어 넣어주세요. 준비되시면 다음이라고 말씀해주세요.";
         break;
      case 4:
         return "가열이 끝나면 오븐에 약 10분 정도 그대로 두어 뜸 들인 후 꺼내어 참기름을 넣고 고루 섞어 줍니다. 이를 위한 알람 셋팅을 하시겠습니까?";
         break;
      case 5:
         return "잣이 완전히 익을 때까지 가열합니다. 다음단계로 진행할까요?";
         break;
      case 6:
         return "오븐에서 꺼낸 다음 잘 식힙니다. 다음단계로 진행할까요?";
         break;
      case 7:
         return "약간 따뜻한 밥을 공모양으로 만든 다음 접시에 내놓습니다. 다음단계로 진행할까요?";
         break;
      default:
         return "end";
         break;
      }
   }
   else if(menu=="deluxe_rice")
   {
      switch(level)
      {
      case 1:
         return "멥쌀, 찹쌀은 씻어서 3시간 이상 불려 놓습니다. 이를 위한 알람 셋팅을 하시겠습니까?";
         break;
      case 2:
         return "밤은 껍질을 벗겨 2등분하고, 잣은 고깔을 떼어 놓습니다. 다음단계로 진행할까요?";
         break;
      case 3:
         return "대추는 씨를 빼고 2등분하고, 콩은 6시간 이상 불려서 준비합니다. 다음단계로 진행할까요?";
         break;
      case 4:
         return "내열용기에 불린 쌀, 콩, 밤, 은행, 대추, 잣, 소금을 넣고 물을 붓습니다. 다음단계로 진행할까요?";
         break;
      case 5:
         return "법랑접시에 석쇠를 놓고 2리터정도 크기의 내열용기를 올린 후 뚜껑 또는 알루미늄 호일로 덮어 1단에 넣어주세요. 준비되시면 다음이라고 말씀해주세요.";
         break;
      case 6:
         return "가열이 끝나면 오븐에 약 10분 정도 그대로 두어 뜸을 들입니다. 이를 위한 알람셋팅을 하시겠습니까?";
         break;
      default:
         return "end";
         break;
      }
   }
   else if(menu=="spaghetti")
   {
      switch(level)
      {
      case 1:
         return "끓는 소금물에 스파게티면을 넣고 삶아 물기 없이 건집니다. 다음단계로 진행할까요?";
         break;
      case 2:
         return "가지는 0.5센티 두께로 둥글게 썰어 소금, 후추, 올리브유를 약간씩 넣고 버무립니다. 다음단계로 진행할까요?";
         break;
      case 3:
         return "팬에 가지를 살짝 볶아 접시에 옮겨 놓고, 올리브유를 팬에 살짝 두르고 다진 마늘과 양파를 넣어 볶아 준 후 갈은 쇠고기, 소금, 후추를 넣고 볶아 양송이, 당근, 샐러리 다진 것을 넣어 익힙니다. 다음단계로 진행할까요?";
         break;
      case 4:
         return "3에 토마토 페이스트와 케첩을 넣고 껍질과 씨를 제거한 토마토를 넣어 준 후 설탕, 핫소스, 월계수 잎, 물을 넣어 끓이면서 소금, 후추로 간을 하고 마지막에 볶아 놓은 가지와 오레가노를 넣어 걸쭉한 상태로 만듭니다. 다음단계로 진행할까요?";
         break;
      case 5:
         return "삶아 놓은 스파게티면을 4에 넣고 버무려 내열접시에 담고 다진 모짜렐라 치즈와 파마산 치즈를 얹어 송송 썬 실파를 뿌립니다. 다음단계로 진행할까요?";
         break;
      case 6:
         return "법랑접시에 석쇠를 놓고 내열접시를 올려주세요. 준비되시면 다음이라고 말씀해주세요.";
         break;
      default:
         return "end";
         break;
      }
   }
   else if(menu=="chocochip")
   {
      switch(level)
      {
      case 1:
         return "박력분, 베이킹파우더, 베이킹 소다는 체쳐 둡니다. 체치기 영상을 보시겠습니까?";
         break;
      case 2:
         return "실온에 둔 버터를 잘 풀어 크림 상태를 만듭니다. 다음단계로 진행할까요?";
         break;
      case 3:
         return "설탕을 넣고 녹을 때까지 잘 섞어줍니다. 다음단계로 진행할까요?";
         break;
      case 4:
         return "달걀을 풀어 조금씩 나누어 넣으며 버터와 분리되지 않도록 재빨리 섞어줍니다. 다음단계로 진행할까요?";
         break;
      case 5:
         return "체친 가루와 초코칩을 넣어 반죽한 후 기름 바른 법랑접시와 구이전용 팬에 적당한 간격을 두고 반 수저씩 놓은 다음 포크로 가볍게 눌러 줍니다. 다음단계로 진행할까요?";
         break;
      case 6:
         return "법랑접시는 1단에, 구이전용 팬은 4단에 각각 넣습니다. 다음단계로 진행할까요?";
         break;
      case 7:
         return "법랑접시와 구이전용 팬을 넣고 준비되시면 다음이라고 말씀해주세요.";
         break;
      default:
         return "end";
         break;
      }
   }
   else if(menu=="cheese_cake")
   {
      switch(level)
      {
      case 1:
         return "크림치즈를 상온에 두어 부드러운 상태가 되도록 한 뒤 거품기를 이용하여 잘 풀어줍니다. 달걀도 미리 풀어둡니다. 다음단계로 진행할까요?";
         break;
      case 2:
         return "1의 크림치즈에 설탕과 달걀을 조금씩 나누어 넣어가며 거품기로 섞습니다. 다음단계로 진행할까요?";
         break;
      case 3:
         return "부드럽게 풀린 반죽에 박력분과 생크림을 넣고 주걱으로 고루 섞습니다. 다음단계로 진행할까요?";
         break;
      case 4:
         return "케이크 틀에 유산지를 깔고 바닥에 카스텔라 시트를 놓은 다음 3의 반죽을 붓습니다. 다음단계로 진행할까요?";
         break;
      case 5:
         return "법랑접시에 석쇠를 놓고 케이크틀을 올린 후 1단에 넣어주세요. 준비되시면 다음이라고 말씀해주세요.";
         break;
      default:
         return "end";
         break;
      }
   }
   else if(menu=="potato")
   {
      switch(level)
      {
      case 1:
         return "감자를 모양이 둥글고 적당한 크기로 골라 깨끗이 씻은 후 물기를 닦아, 껍질을 포크로 몇차례 찔러 줍니다. 다음단계로 진행할까요?";
         break;
      case 2:
         return "법랑접시 위에 석쇠를 놓고 감자를 가지런히 올린 후 넣어주세요. 준비되시면 다음이라고 말씀해주세요.";
         break;
      default:
         return "end";
         break;
      }
   }
   else if(menu=="chicken")
   {
      switch(level)
      {
      case 1:
         return "닭을 씻어서 물기를 닦고 꽁지와 날개 끝을 잘라 내고 닭 껍질에 칼집을 내어 소금, 후추, 청주에 약 30분 정도 재워줍니다. 이를 위한 알람 셋팅을 하시겠습니까?";
         break;
      case 2:
         return "닭 껍질에 올리브유를 바릅니다. 다음단계로 진행할까요?";
         break;
      case 3:
         return "법량접시에 석쇠를 놓고 닭의 가슴부분이 아래로 가도록 하여 넣어주세요. 준비되시면 다음이라고 말씀해주세요.";
         break;
      case 4:
         return "도중에 멜로디가 울리면 한번 뒤집어줍니다. 다음단계로 진행할까요?";
         break;
      default:
         return "end";
         break;
      }
   }
   else if(menu=="pork_belly")
   {
      switch(level)
      {
      case 1:
         return "삼겹살은 통째로 구입하여 2~3개의 덩어리로 자른 후 칼집을 내고 소금, 후추, 올리브유를 적당량 뿌립니다. 다음단계로 진행할까요?";
         break;
      case 2:
         return "법랑접시 위에 석쇠를 놓고 통삼겹살의 껍질부분이 아래로 가도록 하여 1단에 넣어주세요. 준비되시면 다음이라고 말씀해주세요.";
         break;
      default:
         return "end";
         break;
      }
   }
   else if(menu=="pizza")
   {
      switch(level)
      {
      case 1:
         return "크러스트는 볼에 피자 빵가루와 물을 넣고 반죽기로 표면이 매끈해지도록 치댄 후, 반죽을 밀어서 구이전용 팬에 올립니다.  다음단계로 진행할까요?";
         break;
      case 2:
         return "소스는 팬을 달구어 다진 마늘, 양파를 넣고 볶은 후 다진 쇠고기를 넣고 볶은 후, 토마토페이스트를 넣고 잘 섞은 후 육수를 붓고 월계수 잎을 넣어 걸쭉한 상태로 졸이고 오레가노를 넣어 잘 섞어줍니다. 마지막에 소금, 후추로 간합니다. 다음단계로 진행할까요?";
         break;
      case 3:
         return "토핑은 양파, 피망, 양송이는 적당한 크기로 썰어서 살짝 데쳐서 물기를 없앤 후, 베이컨, 햄은 적당한 크기로 자릅니다. 그리고, 모짜렐라 치즈는 잘게 다지고 올리브는 모양대로 얇게 썰어 둡니다. 다음단계로 진행할까요?";
         break;
      case 4:
         return "1의 크러스트 가장자리 1센치 정도만 남기고 가운데 부분은 포크로 찔러주고, 가장자리 1센치 남긴 부분에 올리브유를 살짝 발라 줍니다. 다음단계로 진행할까요?";
         break;
      case 5:
         return "크러스트 가운데 부분의 2의 소스를 고구 펴 바르고 모짜렐라 치즈 50그램을 얇게 뿌립니다. 다음단계로 진행할까요?";
         break;
      case 6:
         return "햄, 베이컨, 양송이, 양파, 피망 순으로 토핑합니다. 다음단계로 진행할까요?";
         break;
      case 7:
         return "마지막으로 나머지 모짜렐라 치즈를 고루 뿌린 뒤 올리브로 장식하여 4단에 넣어주세요. 준비되시면 다음이라고 말씀해주세요.";
         break;
      default:
         return "end";
         break;
      }
   }
   else if(menu=="hamburger")
   {
      switch(level)
      {
      case 1:
         return "팬을 달구어 다진 마늘, 양파를 넣고 볶은 후 한김 식힙니다. 다음단계로 진행할까요?";
         break;
      case 2:
         return "다진 쇠고기, 돼지고기에 다진 마늘, 양파, 달걀, 우유, 빵가루, 소금, 후추, 분유를 넣고 끈기가 생길 때까지 힘껏 치대 줍니다. 다음단계로 진행할까요?";
         break;
      case 3:
         return "끈기가 생긴 고기 반죽을 4등분하여 1.5센치 두께, 지름 8센치로 동글 납작하게 만들어 가운데 부분을 오목하게 눌러 줍니다. 다음단계로 진행할까요?";
         break;
      case 4:
         return "구이전용 팬에 올리브유를 적당히 두르고 가지런히 올린 후 넣어주세요. 준비되시면 다음이라고 말씀해주세요.";
         break;
      case 5:
         return "도중에 멜로디가 울리면 한번 뒤집어 줍니다. 다음단계로 진행할까요?";
         break;
      case 6:
         return "팬을 달구어 다진 마늘, 양파를 넣고 볶은 후 케첩을 넣어 고루 섞이도록 볶아 줍니다. 우스터 소스, 적포도주, 육수를 넣은 후 이분의 일로 될때까지 졸입니다. 다음단계로 진행할까요?";
         break;
      case 7:
         return "우유를 조금씩 넣어 가며 섞어준 후 소금, 후추로 간하여 웨지감자 체에 걸러 햄버그 스테이크에 곁들여 냅니다. 다음단계로 진행할까요?";
         break;
      default:
         return "end";
         break;
      }
   }
   else if(menu=="garlic_bread")
   {
      switch(level)
      {
      case 1:
         return "상온 버터와 다진 마늘, 다진 파슬리를 섞어 식빵의 한 면에 바릅니다. 다음단계로 진행할까요?";
         break;
      case 2:
         return "법량접시에 유산지를 깔고 식빵을 올린 후 넣어주세요. 준비되시면 다음이라고 말씀해주세요.";
         break;
      default:
         return "end";
         break;
      }
   }
   else if(menu=="bagle")
   {
      switch(level)
      {
      case 1:
         return "베이글을 반으로 자릅니다. 다음단계로 진행할까요?";
         break;
      case 2:
         return "베이글의 안쪽 면이 위로 오게하여 구이전용 팬에 겹치지 않게 올리세요. 준비되시면 다음이라고 말씀해 주세요.";
         break;
      default:
         return "end";
         break;
      }
   }
   else if(menu=="bread")
   {
      switch(level)
      {
      case 1:
         return "구이전용 팬에 식빵을 겹치지 않게 올린 후 넣어주세요. 준비되시면 다음이라고 말씀해주세요.";
         break;
      case 2:
         return "도중에 멜로디가 울리면 한번 뒤집어 준다. 준비되시면 다음이라고 말씀해주세요.";
         break;
      default:
         return "end";
         break;
      }
   }
   else if(menu=="pizza_toast")
   {
      switch(level)
      {
      case 1:
         return "토핑은 다음과 같이 만듭니다. 양파,청,홍피망,양송이는 적당한 크기로 썰어서 내열용기에 담고 랩을 씌워 수동요리레인지 기능으로 2~3분 가열한 후 물기를 없앱니다. 이를 위한 알람 셋팅을 하시겠습니까?";
         break;
      case 2:
         return "베이컨, 햄은 적당한 크기로 자릅니다. 다음단계로 진행할까요?";
         break;
      case 3:
         return "모짜렐라 치즈는 잘게 다지고 올리브는 모양대로 얇게 썰어 둡니다. 다음단계로 진행할까요?";
         break;
      case 4:
         return "상온 버터를 식빵의 한 면에 바른 후, 피자소스를 바릅니다. 다음단계로 진행할까요?";
         break;
      case 5:
         return "햄, 베이컨, 양송이, 양파, 피망 순으로 토핑합니다. 다음단계로 진행할까요?";
         break;
      case 6:
         return "모짜렐라 치즈를 고루 뿌린 뒤 올리브로 장식합니다. 다음단계로 진행할까요?";
         break;
      case 7:
         return "법량접시에 유산지를 깔고 식빵을 올린 후 넣어주세요. 준비되시면 다음이라고 말씀해주세요.";
         break;
      default:
         return "end";
         break;
      }
   }
   else if(menu=="honey_bread")
   {
      switch(level)
      {
      case 1:
         return "통 식빵을 반으로 자릅니다. 다음단계로 진행할까요?";
         break;
      case 2:
         return "식빵의 한쪽 면이 위로 오게 하여 밑바닥에 구멍이 나지 않게 엑스자 칼집을 넣어줍니다. 다음단계로 진행할까요?";
         break;
      case 3:
         return "칼집을 낸 부분에 꿀을 넣고 식빵 뒷면에 버터를 바릅니다. 다음단계로 진행할까요?";
         break;
      case 4:
         return "법량접시에 유산지를 깔고 식빵을 올린 후 넣어주세요. 준비되시면 다음이라고 말씀해주세요.";
         break;
      default:
         return "end";
         break;
      }
   }

}

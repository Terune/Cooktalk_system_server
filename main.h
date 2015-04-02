#pragma once
#include "stack.h"
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <map>
#include <curl/curl.h>
//#include "./include/CkHttp.h"
//#include "./include/CkHttpResponse.h"
//#include "./include/CkHttpRequest.h"
using namespace std;
// CTab3 대화 상자입니다.

class CTab3
{
   public:
   virtual bool OnInitDialog();
   string output;
   string output_DA;
   int Materials[55];
   Stack<string> now_act;
   Stack<string> now_slot;
   Stack<string> now_value;
   string By;
   string input_DA;
   string input_slot;
   string input_value;
   map<string,string> slots;
   void postmessage();
   int Check_Input[12];
   void inform_add();
   string inform_In_slot;
   string inform_In_value;
   void confirm_add();
   string Confirm_In_slot;
   string Confirm_In_value;
   string deny_In_slot;
   string deny_In_value;
   void deny_add();
   void request_Get_name(string ComboName);
   void act_working();
   void stack_input(string input,string type);
   protected:
   void ViewNowValue();
   void Doact(string act);
   void Doact(string act,string Slot);
   void Doact(string act,string Slot,string value);
   void deny_Get_name(string ComboName);
   void inform_Get_name(string ComboName);
   void click_init();
   void confirm_Get_name();
   string Menu_Con(string con);
   string Menu_Type(string type);
   string Conv_Korean(string ename);
   string Menu_Material(string menu);
   //CString impl_confirm(CString slot,CString Value);
   string Find_recipe(string menu,int level);
   void get_bymaterial();
   void get_bytype();
   void get_bycon();
   string Find_Material(string language);
   string Find_menu();
   int returnMaterialNum(string input);
   string ComboName;
   string Request_slot;
   bool Requested;
   bool Confirmed;
   bool Denyed;
   string print_act;
   string print_DA;
   string Slot_input;
   int type_point[25];
   int con_point[25];
   int material_point[25];
   string input;
};


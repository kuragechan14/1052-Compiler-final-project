/*
 * (P4) Lexical Analyzer_v1 (�Ĥ���) 2017.06.26
 * Function:
 *	�@	-���J���R�GŪ��statements�A�P�_�r����v�@�ͦ�token�A�ÿ�Xtoken type�Mlexeme
 *		-�簣�ťաB�_�����ѡB���׽u����
 *		-²����syntax error�^��
 * Input:
 *	�@	-PPT����test statements�G
 *			- (s1) a = b + c * d
 *			- (s2) day = (1461*2) + 4.5 + (153*6+2) < 5 + 3
 *			- (s3) 123 =  (123 +* 4 ) 5 * 6
 *			- (s4) �t�_�����ѡA�G���������K�K�W
 *		-�ۭq����G
 *			- a = d * c //Hello 456
 *			- 3..1415=pi
 * Output:
 *		-�̷ӵ{�����涶�ǿ�X���q�ʪ����浲�G
 *			-��(1)���q�G�簣���ѫ᪺�r��
 *			-��(2)���q�G�簣�ťի᪺�r��
 *			-��(3)���q�G��Xstatement�̨̧ǧ��/�ͦ���token <token type,lexeme>
 *		-���t�檩���ΥH��X�����(.txt)
 * Return:
 *		-0:�{������
 */
#include <iostream>
#include <string>
#include <regex>
using namespace std;
string type_list[10]={"","NUM","ID","=","<","<=","+","*","(",")"};
string str;
void del_comment()	//�簣����
{
	string s="";
	for(int i=0;i<str.length();i++)
	{
		if(str[i]=='/' && str[i+1]=='/') {break;}	//�Y���ѬO"//"���榡�A�h��泣����
		else if(str[i]=='/' && str[i+1]=='*')		//�Y���ѬO"/*...*/"���榡
		{
			for(int j=i+2;j<str.length();j++)
			{
				if(str[j-1]=='*' && str[j]=='/')	//�����Ѫ�����
				{
					i=j;	//�j��U�@���N�q�L���U�@�Ӷ}�l�~��B�@
					break;
				}
			}
		}
		else{s+=str[i];}	//��l�r���Ҥ���
	}
	str=s;
	cout<<"(1)�R������:"<<str<<endl;
}
void del_space()	//�簣�ť�
{
	string s="";
	for(int i=0;i<str.length();i++)
	{
		if(str[i]==' '){continue;}	//�J��ťմN���L
		else{s+=str[i];}			//��l�r���Ҥ���
	}
	str=s;
	cout<<"(2)�R���ť�:"<<str<<endl;
}
string next_Token()	//�B�z�Ʀr�B�r��B�Ÿ��A���Xtoken�G�Ѧ�PPT(02)page.101�νҥ�page2-43
{
	
	if(str.length()==0){return "EOF.(1)";}	//��yInput�w�P�_����
	else
	{
		int i(0),t_type(0);
		string peek="";	//��l��peek
		string s="";
		if(isdigit(str[i]))	//�B�z�Ʀr�G�ѦҤ��媩�ҥ���2.6.3�� (page.2-42)
		{	
			peek+=str[i];
			while(i+1<str.length())
			{
				//�wŪ�U�Ӧr���A�Y���Ʀr�N�[�Jpeek
				if(isdigit(str[i+1]) || str[i+1]=='.')
				{
					i++;
					peek+=str[i];
				}
				else{break;}
			}
			//�HRegular Expression�P�_�O�_match NUM���榡
			regex pattern("[0-9]+(\.[0-9]+)?");
			bool match=regex_match(peek,pattern);
			if(match){
				//cout<<"NUM matched - "<<peek<<endl;
				t_type=1;
			}
			else{cout<<peek<<"\tError-NUM not matched."<<endl;}	//��match�ɡA�L�XError Msg
		}
		else if(isalpha(str[i]))	//�B�zID�G�ѦҤ��媩�ҥ���2.6.4�� (page.2-43)
		{
			peek+=str[i];
			while(i+1<str.length())
			{
				//�wŪ�A�Y���r��/�Ʀr/���u�N�[�Jpeek
				if(isalpha(str[i+1]) || isdigit(str[i+1]) || str[i+1]=='_')
				{
					i++;
					peek+=str[i];
				}
				else{break;}
			}
			//�HRE�P�_�O�_match ID���榡
			regex pattern("[a-zA-Z][a-zA-Z_0-9]*");
			bool match=regex_match(peek,pattern);
			if(match){
				//cout<<"ID matched - "<<peek<<endl;
				t_type=2;
			}
			else{cout<<peek<<"\tError-ID not matched."<<endl;}	//��match�ɡA�L�XError Msg
		}
		else if(str[i]=='='){t_type=3;}
		else if(str[i]=='<' && str[i+1]!='='){t_type=4;}
		else if(str[i]=='<' && str[i+1]=='='){t_type=5;i=i+1;}
		else if(str[i]=='+'){t_type=6;}
		else if(str[i]=='*'){t_type=7;}
		else if(str[i]=='('){t_type=8;}
		else if(str[i]==')'){t_type=9;}

		for(int j=i+1;j<str.length();j++) {s+=str[j];}	//��s�|���B�z��statement�r��
		str=s;
		//�YRE�ֹ��L�~�A�h�Φ�token�æ^��token type�Mlexemes (P4��output)
		if(t_type!=0){return "<"+type_list[t_type]+","+peek+">";}
		else {return "EOF.(2)";}	
	}
}
int main()
{
	while(1)
	{
		cout<<"Input the statement:"<<endl;
		getline(cin,str);	//Ū�Jstatement
		if(str=="0"){break;}	//����K��X�ɮסA�]�m��J0�ɡA�����{��

		del_comment();	//�簣����
		del_space();		//�簣�ť�
		cout<<"(3)�̧ǳB�z�Ʀr�B�r��B�Ÿ�:"<<endl;
		while(1)	//���_�I�snext_Token�����s��Token����EOF
		{
			string t=next_Token();
			if(t=="EOF.(1)")	//End of file.
			{
				cout<<"End of file."<<endl;
				break;
			}
			else if(t=="EOF.(2)")	//�Y�J��Error�h��������
			{
				cout<<"[Syntax Error(1)] End of file."<<endl;
				break;
			}
			else{cout<<t<<endl;}	//P4 output�A�]�ttoken type��lexeme
		}
		cout<<endl;
		str="";
	}
	
    return 0;
}
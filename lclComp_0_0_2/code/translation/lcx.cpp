#include "lcx.hpp"
using namespace CompileLCL;

std::string replaceSTR(std::string in,std::string search,std::string replaceStr,char escapeBlockArea){
	char startBlockedArea[] = {'\"'};
	char endBlockedArea[] = {'\"'};
	std::string rt;
	size_t searchIterator = 0;
	size_t startBlockedAreaSize = (sizeof(startBlockedArea) / sizeof(char));
	size_t endBlockedAreaSize = (sizeof(endBlockedArea) / sizeof(char));
	size_t offset = 0;
	bool startFinding = false;
	bool isBlockArea = false;
	size_t startPos = 0;
	for(size_t i =0; i < in.length(); i++){
		if(isBlockArea){
			for(size_t bai = 0; bai < endBlockedAreaSize; bai++){
				if(endBlockedArea[bai] == (char) in[i+offset]){
					if(!(i == 0 || escapeBlockArea == (char) in[i+offset-1]))isBlockArea = false;
					break;
				}
			}
		}
		if(search[searchIterator] == in[i + offset] && !isBlockArea){
			if(!startFinding)startPos = i;
			startFinding = true;
			searchIterator++;
		//	if(search == "static") std::cout << "STA:" << rt << in << std::endl;
			if(searchIterator == search.length()){
				rt += replaceStr;
				startFinding = false;
				searchIterator = 0;
			}
		}else{
		//	if(search == "static") std::cout << "STA:" << searchIterator << rt << in << std::endl;
			if(startFinding){
				startFinding = false;
				i = startPos;
				searchIterator =0;
			}
			rt += in[i+offset];
		}
		if(!isBlockArea){
			for(size_t bai = 0; bai < startBlockedAreaSize; bai++){
				if(startBlockedArea[bai] == (char) in[i+offset]){
					if(!(i == 0 || escapeBlockArea == (char) in[i+offset-1]))isBlockArea = true;
					break;
				}
			}
		}
	}
	return rt;
}

std::string LCX::translate(std::string in){
	for(size_t i = 0; i< in.length(); i++){
		if((int)'A' <= (int) in.at(i) && (int) in.at(i) <= (int) 'Z'){in.insert(i,"#"); i++;}
	}
    for(long long i = 0; i < Keywords::length; i++){
            in = replaceSTR(in,LCX::m[i],Keywords::m[i],'\\');
			//std::cout << "RP: " << LCX::m[i] << ">> " << Keywords::m[i] << " || " << in;
    }
    return in;
}
void LCX::init(Keywords::l a,std::string b){
    m[a] = b;
}
void LCX::reg(){
	LCX::init(Keywords::l::para_public,"public");
	LCX::init(Keywords::l::para_private,"private");
	LCX::init(Keywords::l::para_protected,"protected");
//
	LCX::init(Keywords::l::para_final,"final");
	LCX::init(Keywords::l::para_static,"static");
	LCX::init(Keywords::l::para_abstract,"abstract");
	LCX::init(Keywords::l::para_transient,"transient");
	LCX::init(Keywords::l::para_volatile,"volatile");
	LCX::init(Keywords::l::para_strictfp,"strictfp");
//
	LCX::init(Keywords::l::para_int," int ");
	LCX::init(Keywords::l::para_boolean," bool ");
	LCX::init(Keywords::l::para_char," char ");
	LCX::init(Keywords::l::para_string," string ");
	LCX::init(Keywords::l::para_byte," byte ");
	LCX::init(Keywords::l::para_float," float ");
	LCX::init(Keywords::l::para_long," long ");
	LCX::init(Keywords::l::para_double," double ");
	LCX::init(Keywords::l::para_short," short ");
	LCX::init(Keywords::l::para_object," X ");
	LCX::init(Keywords::l::para_void," void ");
	LCX::init(Keywords::l::para_ab_obj_cl, " abObj ");
//
	LCX::init(Keywords::l::para_plus,"+");
	LCX::init(Keywords::l::para_plus_set,"+=");
	LCX::init(Keywords::l::para_plus_plus,"++");
	LCX::init(Keywords::l::para_minus,"-");
	LCX::init(Keywords::l::para_minus_set,"-=");
	LCX::init(Keywords::l::para_minus_minus,"--");
	LCX::init(Keywords::l::para_multiply,"*");
	LCX::init(Keywords::l::para_multiply_set,"*=");
	LCX::init(Keywords::l::para_divide,"/");
	LCX::init(Keywords::l::para_divide_set,"/=");
	LCX::init(Keywords::l::para_modulo,"%");
	LCX::init(Keywords::l::para_modulo_set,"%=");
    LCX::init(Keywords::l::para_power_of,"**");
		//
	LCX::init(Keywords::l::para_set,"=");
	LCX::init(Keywords::l::para_equal,"==");
	LCX::init(Keywords::l::para_not_equal,"!=");
	LCX::init(Keywords::l::para_greater_than,">");
	LCX::init(Keywords::l::para_smaller_than,"<");
	LCX::init(Keywords::l::para_greater_equal,">=");
	LCX::init(Keywords::l::para_smaller_equal,"<=");
	LCX::init(Keywords::l::para_not,"!");
	LCX::init(Keywords::l::para_and_and,"&&");
	LCX::init(Keywords::l::para_or_or,"||");
	LCX::init(Keywords::l::para_xor,"^");
	LCX::init(Keywords::l::para_unary,"~");
	LCX::init(Keywords::l::para_right_shift,">>");
	LCX::init(Keywords::l::para_right_right_shift,">>>");
	LCX::init(Keywords::l::para_left_shift,"<<");
	LCX::init(Keywords::l::para_and_set,"&=");
	LCX::init(Keywords::l::para_or_set,"/=");
	LCX::init(Keywords::l::para_xor_set,"^=");
	LCX::init(Keywords::l::para_right_shift_set,">>=");
	LCX::init(Keywords::l::para_right_right_shift_set,">>>=");
	LCX::init(Keywords::l::para_left_shift_set,"<<=");
	LCX::init(Keywords::l::para_object_or,"#X|");
	//
	LCX::init(Keywords::l::para_if,"?");
	LCX::init(Keywords::l::para_else,":");
	LCX::init(Keywords::l::para_for,"for");
	LCX::init(Keywords::l::para_while,"while");
	LCX::init(Keywords::l::para_do,"do");
	LCX::init(Keywords::l::para_switch,"switch");
	LCX::init(Keywords::l::para_if_end,"::");
	LCX::init(Keywords::l::para_return,"return");
	LCX::init(Keywords::l::para_break,"break");
	LCX::init(Keywords::l::para_continue,"continue");
	LCX::init(Keywords::l::para_case,"case");
	LCX::init(Keywords::l::para_default,"default");
	LCX::init(Keywords::l::para_goto,"goto");
	LCX::init(Keywords::l::para_goto_allowance,"allowGT");
	//
	LCX::init(Keywords::l::para_bracket_round_open,"(");
	LCX::init(Keywords::l::para_bracket_round_close,")");
	LCX::init(Keywords::l::para_bracket_cornered_open,"[");
	LCX::init(Keywords::l::para_bracket_cornered_close,"]");
	LCX::init(Keywords::l::para_bracket_curly_open,"{");
	LCX::init(Keywords::l::para_bracket_curly_close,"}");
	//
	LCX::init(Keywords::l::para_new,"new");
	LCX::init(Keywords::l::para_exit,"exit");
	//LCX::init(Keywords::l::para_escape_uppercase,"#");
	LCX::init(Keywords::l::para_semicolon,";");
	LCX::init(Keywords::l::para_quotes,"\"");
	LCX::init(Keywords::l::para_super,"super");
	LCX::init(Keywords::l::para_annotation,"@");
	LCX::init(Keywords::l::para_this,"this");
	LCX::init(Keywords::l::para_const,"const");
	//
	LCX::init(Keywords::l::para_comment_area_start,"/*");
	LCX::init(Keywords::l::para_comment_area_end,"*/");
	LCX::init(Keywords::l::para_comment_line,"//");
	//
	LCX::init(Keywords::l::para_class,"class ");
	LCX::init(Keywords::l::para_interface,"interface");
	LCX::init(Keywords::l::para_nativ,"nativ");
	LCX::init(Keywords::l::para_enum,"enum");
	LCX::init(Keywords::l::para_record,"record");
	LCX::init(Keywords::l::para_catch,"catch");
	LCX::init(Keywords::l::para_finally,"finally");
	LCX::init(Keywords::l::para_thro,"throw");
	LCX::init(Keywords::l::para_throws,"throws");
	LCX::init(Keywords::l::para_try,"try");
	LCX::init(Keywords::l::para_assert,"assert");
	//
	LCX::init(Keywords::l::para_package,"pack");
	LCX::init(Keywords::l::para_import,"import");
	//
	LCX::init(Keywords::l::para_synchronized,"synchronized");
	LCX::init(Keywords::l::para_comma,",");
	LCX::init(Keywords::l::para_point,".");
	//
//	LCX::init(Keywords::l::para_,"");
}
#ifndef KEYWORDS_H_
#define KEYWORDS_H_
#include <string>
#define OperatorOverload "op"
/**
 * In general if a Keyword in the enum is part of a longer keyword it needs to go below the longer keywords,
 * the lcl Compiler does not care, but the lcx translation otherwise translates / replaces it wrong
 * e.g.
 * the != Keywords is replaced first the ! to N then the = to V => NV => But of cause this is not correct,
 * so it needs to search for the != Keyword before the ! or = Keywords
*/
namespace CompileLCL
{
	namespace Keywords
	{
		extern int c;
		enum l
		{
			// To ensure to not Override string contol characters
			offset_0,
			offset_1,
			offset_2,
			offset_3,
			offset_4,
			offset_5,
			offset_6,
			offset_7,
			offset_8,
			null,
			number,
			positiv_int,
			negativ_int,
			positiv_float,
			negativ_float,
			context_string,
			actual_string,
			start_of_enum,
			para_public,
			para_private,
			para_protected,
            //
            para_comment_line,
            para_comment_area_start,
            para_comment_area_end,
			//
			para_final,
			para_static,
			para_abstract,
			para_transient,
			para_volatile,
			para_strictfp,
			para_goto_allowance,
			//
			para_int,
			para_boolean,
			para_char,
			para_string,
			para_byte,
			para_float,
			para_long,
			para_double,
			para_short,
			para_object,
			para_void,
			para_ab_obj_cl,
			//
			para_plus_set,
			para_plus_plus,
			para_plus,
			para_minus_set,
			para_minus_minus,
			para_minus,
            para_power_of,
			para_multiply_set,
			para_multiply,
			para_divide_set,
			para_divide,
			para_modulo_set,
			para_modulo,
			//
			para_not_equal,
			para_equal,
			para_greater_than,
			para_smaller_than,
			para_greater_equal,
			para_smaller_equal,
			para_not,
			para_set,
			para_and_and,
			para_or_or,
			para_xor,
			para_unary,
			para_right_shift,
			para_right_right_shift,
			para_left_shift,
			para_and_set,
			para_or_set,
			para_or,
			para_and,
			para_xor_set,
			para_right_shift_set,
			para_right_right_shift_set,
			para_left_shift_set,
			para_object_or,
			//
			para_if,
			para_else,
			para_for,
			para_while,
			para_do,
			para_switch,
			para_if_end,
			para_return,
			para_break,
			para_continue,
			para_case,
			para_default,
			para_goto,
			//
			para_bracket_round_open,
			para_bracket_round_close,
			para_bracket_cornered_open,
			para_bracket_cornered_close,
			para_bracket_curly_open,
			para_bracket_curly_close,
			//
			para_new,
			para_exit,
			//para_escape_uppercase,
			para_semicolon,
			para_quotes,
			para_super,
			para_annotation,
			para_this,
			para_const,

			para_class,
			para_interface,
			para_nativ,
			para_enum,
			para_record,
			//
			para_catch,
			para_finally,
			para_thro,
			para_throws,
			para_try,
			para_assert,
			//
			para_package,
			para_import,
			//
			para_synchronized,
			para_comma,
			para_point,
			//
			para_pointer_addr,
			para_pointer_value,
            //
            para_extern,
			//
			end_of_enum
		};
		const int length = end_of_enum;
		extern std::string m[length];
		void init(const int addr, std::string x);
		void reg();
	}
}
#endif

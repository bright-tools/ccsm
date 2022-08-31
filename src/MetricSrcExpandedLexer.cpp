/*
   Copyright 2014 John Bailey

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "MetricSrcExpandedLexer.hpp"
#include "MetricUtils.hpp"
#include "clang/AST/ASTContext.h"
#include "clang/Lex/Preprocessor.h"

#include <iostream>

#define SOURCE_MANAGER (m_compilerInstance.getSourceManager())

const static std::pair<MetricType_e, MetricType_e> metricToBodyMetricMapData[] = {
    std::make_pair(METRIC_TYPE_DOUBLE, METRIC_TYPE_BODY_DOUBLE),
    std::make_pair(METRIC_TYPE_FLOAT, METRIC_TYPE_BODY_FLOAT),
    std::make_pair(METRIC_TYPE_CONST, METRIC_TYPE_BODY_CONST),
    std::make_pair(METRIC_TYPE_CHAR, METRIC_TYPE_BODY_CHAR),
    std::make_pair(METRIC_TYPE_SHORT, METRIC_TYPE_BODY_SHORT),
    std::make_pair(METRIC_TYPE_LONG, METRIC_TYPE_BODY_LONG),
    std::make_pair(METRIC_TYPE_UNSIGNED, METRIC_TYPE_BODY_UNSIGNED),
    std::make_pair(METRIC_TYPE_SIGNED, METRIC_TYPE_BODY_SIGNED),
    std::make_pair(METRIC_TYPE_STATIC, METRIC_TYPE_BODY_STATIC),
    std::make_pair(METRIC_TYPE_UNION, METRIC_TYPE_BODY_UNION),
    std::make_pair(METRIC_TYPE_ENUM, METRIC_TYPE_BODY_ENUM),
    std::make_pair(METRIC_TYPE_VOID, METRIC_TYPE_BODY_VOID),
    std::make_pair(METRIC_TYPE_STRUCT, METRIC_TYPE_BODY_STRUCT),
    std::make_pair(METRIC_TYPE_INT, METRIC_TYPE_BODY_INT)};

const std::map<MetricType_e, MetricType_e> MetricSrcExpandedLexer::m_metricToBodyMetricMap(
    metricToBodyMetricMapData, metricToBodyMetricMapData + sizeof metricToBodyMetricMapData /
                                                               sizeof metricToBodyMetricMapData[0]);

const static std::pair<clang::tok::TokenKind, MetricType_e> tokenKindToTypeMapData[] = {
#if 0
	/* These are handed in metric matcher such that the difference between unary and non-unary operators can be 
	   distinguished */
	std::make_pair(clang::tok::plus,         METRIC_TYPE_OPERATOR_ARITHMETIC_ADDITION),
	std::make_pair(clang::tok::minus,        METRIC_TYPE_OPERATOR_ARITHMETIC_SUBTRACTION),

	/* This is handled in MetricMatcher such that the difference between multiplication and pointer de-reference
	   can be distinguished */
	std::make_pair(clang::tok::star,         METRIC_TYPE_OPERATOR_ARITHMETIC_MULTIPLICATION),

	/* This is handled in MetricMatcher such that the difference between a bitwise AND and address-of can be
	   distinguished */
	std::make_pair(clang::tok::amp,          METRIC_TYPE_OPERATOR_BITWISE_AND),

	/* These are handled in MetricMatcher such that the difference between array subscripts and array declarations
	   can be distinguished */
	std::make_pair(clang::tok::l_square,            METRIC_TYPE_TOKEN_LSQUARE),
	std::make_pair(clang::tok::r_square,            METRIC_TYPE_TOKEN_RSQUARE),

	/* These are handled in MetricMatcher such that the difference between brackets for use in arithmetic precedence 
	   and function calls can be distinguished */
	std::make_pair(clang::tok::l_paren,      METRIC_TYPE_TOKEN_LPAREN),
	std::make_pair(clang::tok::r_paren,      METRIC_TYPE_TOKEN_RPAREN),
#endif
#if 0
	/* C99/C++ keywords */
	//	std::make_pair(clang::tok::kw_asm,       METRIC_TYPE_TOKEN_ASM),
	//	std::make_pair(clang::tok::kw_bool,      METRIC_TYPE_TOKEN_BOOL),
	//	std::make_pair(clang::tok::kw_friend,    METRIC_TYPE_TOKEN_FRIEND),
	//	std::make_pair(clang::tok::kw_virtual,   METRIC_TYPE_TOKEN_VIRTUAL),
	//	std::make_pair(clang::tok::kw_mutable,   METRIC_TYPE_TOKEN_MUTABLE),
	//	std::make_pair(clang::tok::kw_class,     METRIC_TYPE_TOKEN_CLASS),
	//	std::make_pair(clang::tok::kw_delete,    METRIC_TYPE_TOKEN_DELETE),
	//	std::make_pair(clang::tok::kw_this,      METRIC_TYPE_TOKEN_THIS),
	//	std::make_pair(clang::tok::kw_new,       METRIC_TYPE_TOKEN_NEW),
	//	std::make_pair(clang::tok::kw_operator,  METRIC_TYPE_TOKEN_OPERATOR),
	//	std::make_pair(clang::tok::kw_private,   METRIC_TYPE_TOKEN_PRIVATE),
	//	std::make_pair(clang::tok::kw_protected, METRIC_TYPE_TOKEN_PROTECTED),
	//	std::make_pair(clang::tok::kw_public,    METRIC_TYPE_TOKEN_PUBLIC),
	//	std::make_pair(clang::tok::kw_namespace,        METRIC_TYPE_TOKEN_NAMESPACE),
	//	std::make_pair(clang::tok::kw_using,            METRIC_TYPE_TOKEN_USING),
	//	std::make_pair(clang::tok::kw_try,              METRIC_TYPE_TOKEN_TRY),
	//	std::make_pair(clang::tok::kw_catch,            METRIC_TYPE_TOKEN_CATCH),
	//	std::make_pair(clang::tok::kw_throw,            METRIC_TYPE_TOKEN_THROW),
	//	std::make_pair(clang::tok::kw_typeid,           METRIC_TYPE_TOKEN_TYPEID),
	//	std::make_pair(clang::tok::kw_template,         METRIC_TYPE_TOKEN_TEMPLATE),
	//	std::make_pair(clang::tok::kw_explicit,         METRIC_TYPE_TOKEN_EXPLICIT),
	//	std::make_pair(clang::tok::kw_true,             METRIC_TYPE_TOKEN_TRUE),
	//	std::make_pair(clang::tok::kw_false,            METRIC_TYPE_TOKEN_FALSE),
	//	std::make_pair(clang::tok::kw_typename,         METRIC_TYPE_TOKEN_TYPENAME),
	//	std::make_pair(clang::tok::coloncolon,          METRIC_TYPE_TOKEN_COLONCOLON),
	//	std::make_pair(clang::tok::kw_inline,    METRIC_TYPE_TOKEN_INLINE),
#endif
#if 0
	/* Don't currently see the value in counting these */
	//	std::make_pair(clang::tok::l_brace, METRIC_TYPE_TOKEN_LBRACE),
	//  std::make_pair(clang::tok::r_brace, METRIC_TYPE_TOKEN_RBRACE),
#endif
#if 0
	/* TODO: Determine rationale for counting these in MetricMatcher rather than here */
	//	std::make_pair(clang::tok::question,            METRIC_TYPE_TOKEN_QUESTION),
	//	std::make_pair(clang::tok::tilde,               METRIC_TYPE_TOKEN_TILDE)
	//	std::make_pair(clang::tok::plusplus,     METRIC_TYPE_TOKEN_PLUSPLUS),
	//	std::make_pair(clang::tok::comma,        METRIC_TYPE_TOKEN_COMMA),
	//	std::make_pair(clang::tok::minusminus,   METRIC_TYPE_TOKEN_MINUSMINUS),
	//	std::make_pair(clang::tok::arrow,        METRIC_TYPE_TOKEN_MEMBER_POINTER),
	//	std::make_pair(clang::tok::period,       METRIC_TYPE_TOKEN_MEMBER_REF),
	//	std::make_pair(clang::tok::ellipsis,     METRIC_TYPE_TOKEN_ELLIPSIS),
	//	std::make_pair(clang::tok::colon,        METRIC_TYPE_TOKEN_COLON),
	//	std::make_pair(clang::tok::exclaim,      METRIC_TYPE_TOKEN_NOT),
#endif
    std::make_pair(clang::tok::exclaimequal, METRIC_TYPE_OPERATOR_COMP_NOT_EQUAL),
    std::make_pair(clang::tok::percent, METRIC_TYPE_OPERATOR_ARITHMETIC_MODULO),
    std::make_pair(clang::tok::percentequal, METRIC_TYPE_OPERATOR_ARITHMETIC_MODULO_ASSIGN),
    std::make_pair(clang::tok::ampamp, METRIC_TYPE_OPERATOR_LOGICAL_AND),
    std::make_pair(clang::tok::pipepipe, METRIC_TYPE_OPERATOR_LOGICAL_OR),
    std::make_pair(clang::tok::ampequal, METRIC_TYPE_OPERATOR_BITWISE_AND_ASSIGN),
    std::make_pair(clang::tok::starequal, METRIC_TYPE_OPERATOR_ARITHMETIC_MULTIPLICATION_ASSIGN),
    std::make_pair(clang::tok::plusequal, METRIC_TYPE_OPERATOR_ARITHMETIC_ADDITION_ASSIGN),
    std::make_pair(clang::tok::minusequal, METRIC_TYPE_OPERATOR_ARITHMETIC_SUBTRACTION_ASSIGN),
    std::make_pair(clang::tok::slash, METRIC_TYPE_OPERATOR_ARITHMETIC_DIVISION),
    std::make_pair(clang::tok::slashequal, METRIC_TYPE_OPERATOR_ARITHMETIC_DIVISION_ASSIGN),
    std::make_pair(clang::tok::kw_auto, METRIC_TYPE_AUTO),
    std::make_pair(clang::tok::kw_for, METRIC_TYPE_FORLOOP),
    std::make_pair(clang::tok::kw_extern, METRIC_TYPE_EXTERN),
    std::make_pair(clang::tok::kw_register, METRIC_TYPE_REGISTER),
    std::make_pair(clang::tok::kw_if, METRIC_TYPE_IF),
    std::make_pair(clang::tok::kw_else, METRIC_TYPE_ELSE),
    std::make_pair(clang::tok::kw_char, METRIC_TYPE_CHAR),
    std::make_pair(clang::tok::kw_double, METRIC_TYPE_DOUBLE),
    std::make_pair(clang::tok::kw_float, METRIC_TYPE_FLOAT),
    std::make_pair(clang::tok::kw_int, METRIC_TYPE_INT),
    std::make_pair(clang::tok::kw_long, METRIC_TYPE_LONG),
    std::make_pair(clang::tok::kw_short, METRIC_TYPE_SHORT),
    std::make_pair(clang::tok::kw_signed, METRIC_TYPE_SIGNED),
    std::make_pair(clang::tok::kw_unsigned, METRIC_TYPE_UNSIGNED),
    std::make_pair(clang::tok::kw_void, METRIC_TYPE_VOID),
    std::make_pair(clang::tok::kw_const, METRIC_TYPE_CONST),
    std::make_pair(clang::tok::kw_volatile, METRIC_TYPE_VOLATILE),
    std::make_pair(clang::tok::kw_static, METRIC_TYPE_STATIC),
    std::make_pair(clang::tok::kw_typedef, METRIC_TYPE_TYPEDEF),
    std::make_pair(clang::tok::kw_break, METRIC_TYPE_BREAK),
    std::make_pair(clang::tok::kw_case, METRIC_TYPE_CASE),
    std::make_pair(clang::tok::kw_continue, METRIC_TYPE_CONTINUE),
    std::make_pair(clang::tok::kw_default, METRIC_TYPE_DEFAULT),
    std::make_pair(clang::tok::kw_do, METRIC_TYPE_DO),
    std::make_pair(clang::tok::kw_enum, METRIC_TYPE_ENUM),
    std::make_pair(clang::tok::kw_goto, METRIC_TYPE_GOTO),
    std::make_pair(clang::tok::kw_return, METRIC_TYPE_RETURN),
    std::make_pair(clang::tok::kw_sizeof, METRIC_TYPE_OPERATOR_SIZE_OF),
    std::make_pair(clang::tok::kw_struct, METRIC_TYPE_STRUCT),
    std::make_pair(clang::tok::kw_switch, METRIC_TYPE_SWITCH),
    std::make_pair(clang::tok::kw_union, METRIC_TYPE_UNION),
    std::make_pair(clang::tok::kw_while, METRIC_TYPE_WHILE),
    std::make_pair(clang::tok::less, METRIC_TYPE_OPERATOR_COMP_LESS_THAN),
    std::make_pair(clang::tok::lessless, METRIC_TYPE_OPERATOR_SHIFT_LEFT),
    std::make_pair(clang::tok::lesslessequal, METRIC_TYPE_OPERATOR_SHIFT_LEFT_ASSIGN),
    std::make_pair(clang::tok::lessequal, METRIC_TYPE_OPERATOR_COMP_LESS_THAN_EQUAL),
    std::make_pair(clang::tok::equal, METRIC_TYPE_OPERATOR_ARITHMETIC_ASSIGN),
    std::make_pair(clang::tok::equalequal, METRIC_TYPE_OPERATOR_COMP_EQUAL),
    std::make_pair(clang::tok::greater, METRIC_TYPE_OPERATOR_COMP_GREATER_THAN),
    std::make_pair(clang::tok::greaterequal, METRIC_TYPE_OPERATOR_COMP_GREATER_THAN_EQUAL),
    std::make_pair(clang::tok::greatergreater, METRIC_TYPE_OPERATOR_SHIFT_RIGHT),
    std::make_pair(clang::tok::greatergreaterequal, METRIC_TYPE_OPERATOR_SHIFT_RIGHT_ASSIGN),
    std::make_pair(clang::tok::caret, METRIC_TYPE_OPERATOR_BITWISE_XOR),
    std::make_pair(clang::tok::caretequal, METRIC_TYPE_OPERATOR_BITWISE_XOR_ASSIGN),
    std::make_pair(clang::tok::pipe, METRIC_TYPE_OPERATOR_BITWISE_OR),
    std::make_pair(clang::tok::pipeequal, METRIC_TYPE_OPERATOR_BITWISE_OR_ASSIGN),
};

const std::map<clang::tok::TokenKind, MetricType_e> MetricSrcExpandedLexer::m_tokenKindToTypeMap(
    tokenKindToTypeMapData,
    tokenKindToTypeMapData + sizeof tokenKindToTypeMapData / sizeof tokenKindToTypeMapData[0]);

MetricSrcExpandedLexer::MetricSrcExpandedLexer(clang::CompilerInstance &p_CI, MetricUnit *p_topUnit,
                                               MetricOptions &p_options)
    : MetricSrcLexer(p_CI, p_topUnit, p_options) {
}

MetricSrcExpandedLexer::~MetricSrcExpandedLexer(void) {
}

MetricUnitProcessingType_e MetricSrcExpandedLexer::getLexType(void) const {
    return METRIC_UNIT_PROCESS_LEX_EXPANDED;
}

void MetricSrcExpandedLexer::HandleOperand(clang::Token &p_token) {
}

void MetricSrcExpandedLexer::ProcessToken(clang::Token &p_token) {
    std::string tok_data;
    unsigned int tok_len = p_token.getLength();
    const clang::SourceLocation startLoc = p_token.getLocation();
    const SourceFileAndLine_t fileAndLineLoc = getFileAndLine(SOURCE_MANAGER, &startLoc);

    if (m_options.getDumpTokens()) {
        m_options.getOutput() << "(" << p_token.getName();
    }

    switch (p_token.getKind()) {
        case clang::tok::numeric_constant:
            tok_data = clang::StringRef(p_token.getLiteralData(), tok_len).str();
            m_currentFnNumerics.insert(tok_data);
            m_currentUnit->increment(METRIC_TYPE_NUMERIC_CONSTANTS, fileAndLineLoc);
            break;
        case clang::tok::char_constant:
            tok_data = clang::StringRef(p_token.getLiteralData(), tok_len).str();
            m_currentFnCharConsts.insert(tok_data);
            m_currentUnit->increment(METRIC_TYPE_CHAR_CONSTS, fileAndLineLoc);
            break;
        case clang::tok::string_literal:
            tok_data = clang::StringRef(p_token.getLiteralData(), tok_len).str();
            m_currentFnStrings.insert(tok_data);
            m_currentUnit->increment(METRIC_TYPE_STRING_LITERALS, fileAndLineLoc);
            break;
        case clang::tok::comment:
            m_currentUnit->increment(METRIC_TYPE_COMMENT_COUNT, fileAndLineLoc, 1);
            m_currentUnit->increment(METRIC_TYPE_COMMENT_BYTE_COUNT, fileAndLineLoc, tok_len);
            break;
        case clang::tok::eof:
            /* Not interested in registering end-of-file */
            break;
        default:
            std::map<clang::tok::TokenKind, MetricType_e>::const_iterator typeLookup =
                m_tokenKindToTypeMap.find(p_token.getKind());

            if (typeLookup != m_tokenKindToTypeMap.end()) {
                m_currentUnit->increment((*typeLookup).second, fileAndLineLoc);
                if (m_inBody) {
                    std::map<MetricType_e, MetricType_e>::const_iterator bodyTypeLookup =
                        m_metricToBodyMetricMap.find((*typeLookup).second);
                    if (bodyTypeLookup != m_metricToBodyMetricMap.end()) {
                        m_currentUnit->increment((*bodyTypeLookup).second, fileAndLineLoc);
                    }
                }
            } else {
                if (p_token.isAnyIdentifier()) {
                    std::string tok_data = p_token.getIdentifierInfo()->getName().str();
                    if (m_options.getDumpTokens()) {
                        m_options.getOutput() << ",unreserved:" << tok_data;
                    }
                    m_currentFnIdentifiers.insert(tok_data);
                    m_currentUnit->increment(METRIC_TYPE_UNRESERVED_IDENTIFIERS, fileAndLineLoc);
                    if (m_inBody) {
                        m_currentBodyIdentifiers.insert(tok_data);
                        m_currentUnit->increment(METRIC_TYPE_BODY_UNRESERVED_IDENTIFIERS,
                                                 fileAndLineLoc);
                    }
                } else {
                    /* TODO */
                }
                /* TODO */
            }

            break;
    }

    HandleOperand(p_token);

    if (m_options.getDumpTokens()) {
        m_options.getOutput() << "," << p_token.getLocation().getRawEncoding();
        m_options.getOutput() << ","
                              << m_compilerInstance.getSourceManager()
                                     .getFileLoc(p_token.getLocation())
                                     .getRawEncoding();
        if (tok_data.length()) {
            m_options.getOutput() << "," << tok_data;
        }
        m_options.getOutput() << "," << (long)(p_token.getKind());
        m_options.getOutput() << "," << (long)(p_token.getFlags());
        m_options.getOutput() << ")";
        if (m_dumpNewline) {
            m_options.getOutput() << std::endl << "  ";
            m_dumpNewline = false;
        }
    }
}

void MetricSrcExpandedLexer::CloseOutFnOrMtd(void) {
    /* Have a current unit? */
    if (m_currentUnit != NULL) {
        /* Close off accumulated metrics */
        m_currentUnit->setSupplementary(METRIC_TYPE_NUMERIC_CONSTANTS, m_currentFnNumerics);
        m_currentUnit->setSupplementary(METRIC_TYPE_STRING_LITERALS, m_currentFnStrings);
        m_currentUnit->setSupplementary(METRIC_TYPE_CHAR_CONSTS, m_currentFnCharConsts);
        m_currentUnit->setSupplementary(METRIC_TYPE_UNRESERVED_IDENTIFIERS, m_currentFnIdentifiers);
        m_currentUnit->setSupplementary(METRIC_TYPE_BODY_UNRESERVED_IDENTIFIERS,
                                        m_currentBodyIdentifiers);
    }
    m_currentFnNumerics.clear();
    m_currentFnStrings.clear();
    m_currentFnIdentifiers.clear();
    m_currentBodyIdentifiers.clear();
    m_currentFnCharConsts.clear();
}

void MetricSrcExpandedLexer::EnterFileScope(void) {
    m_currentFnNumerics = m_currentUnit->getSupplementary(METRIC_TYPE_NUMERIC_CONSTANTS);
    m_currentFnStrings = m_currentUnit->getSupplementary(METRIC_TYPE_STRING_LITERALS);
    m_currentFnCharConsts = m_currentUnit->getSupplementary(METRIC_TYPE_CHAR_CONSTS);
    m_currentFnIdentifiers = m_currentUnit->getSupplementary(METRIC_TYPE_UNRESERVED_IDENTIFIERS);
}
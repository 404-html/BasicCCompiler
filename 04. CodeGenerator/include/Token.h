#pragma once

#include <string>
#include <map>
#include <assert.h>

#define NOT !
#define GET_INFO_FOR_KEY(__node__, __key__)					__node__->getAdditionalInfoFor(__key__)
#define SET_INFO_FOR_KEY(__node__, __key__, __info__)		__node__->setAdditionalInfo(__key__, __info__)

namespace TokenType
{
	enum class Type
	{
		TK_INVALID = -1,
		TK_MUL,
		TK_MULEQ,
		TK_DIV,
		TK_DIVEQ,
		TK_MOD,
		TK_ADD,
		TK_ADDEQ,
		TK_SUB,
		TK_SUBEQ,
		TK_NEGATE,
		TK_NOT,
		TK_LT,
		TK_LTEQ,
		TK_GT,
		TK_GTEQ,
		TK_EQ,
		TK_NEQ,
		TK_ASSIGN,
		TK_LOGICALAND,
		TK_LOGICALOR,
		TK_BITWISEAND,
		TK_BITWISEOR,
		TK_BITWISEXOR,
		TK_BITWISENOT,
		TK_BITWISELEFTSHIFT,
		TK_BITWISERIGHTSHIFT,
		TK_IF,
		TK_ELSE,
		TK_FOR,
		TK_WHILE,
		TK_PRINT,
		TK_LPAREN,
		TK_RPAREN,
		TK_LBRACE,
		TK_RBRACE,
		TK_LSQBRACKET,
		TK_RSQBRACKET,
		TK_SEMICOL,
		TK_COMMA,
		TK_IDENTIFIER,
		TK_INTEGER,
		TK_FLOAT,
		TK_STRING,
		TK_CHARACTER,
		TK_EOL,
		TK_EOI,
		TK_SINGLELINECOMMENT,
		TK_MULTILINECOMMENT,
		TK_WHITESPACE,
		TK_BNFNONTERMINAL,
		TK_BNFASSIGNMENT,
		TK_BNFCODE,
		TK_FUNCTIONCALL,
		TK_PREFIXDECR,
		TK_PREFIXINCR,
		TK_POSTFIXDECR,
		TK_POSTFIXINCR,
		TK_SWITCH,
		TK_DEREF,
		TK_DEREFARRAY,
		TK_MEMBERACCESS,
		TK_UNKNOWN
	};

	inline std::string toString(Type eType)
	{
		switch (eType)
		{
		case Type::TK_INVALID:				return "TK_INVALID";
		case Type::TK_MUL:					return "TK_MUL";
		case Type::TK_MULEQ:				return "TK_MULEQ";
		case Type::TK_DIV:					return "TK_DIV";
		case Type::TK_DIVEQ:				return "TK_DIVEQ";
		case Type::TK_MOD:					return "TK_MOD";
		case Type::TK_ADD:					return "TK_ADD";
		case Type::TK_ADDEQ:				return "TK_ADDEQ";
		case Type::TK_SUB:					return "TK_SUB";
		case Type::TK_SUBEQ:				return "TK_SUBEQ";
		case Type::TK_NEGATE:				return "TK_NEGATE";
		case Type::TK_NOT:					return "TK_NOT";
		case Type::TK_LT:					return "TK_LT";
		case Type::TK_LTEQ:					return "TK_LTEQ";
		case Type::TK_GT:					return "TK_GT";
		case Type::TK_GTEQ:					return "TK_GTEQ";
		case Type::TK_EQ:					return "TK_EQ";
		case Type::TK_NEQ:					return "TK_NEQ";
		case Type::TK_ASSIGN:				return "TK_ASSIGN";
		case Type::TK_LOGICALAND:			return "TK_LOGICALAND";
		case Type::TK_LOGICALOR:			return "TK_LOGICALOR";
		case Type::TK_BITWISEAND:			return "TK_BITWISEAND";
		case Type::TK_BITWISEOR:			return "TK_BITWISEOR";
		case Type::TK_BITWISEXOR:			return "TK_BITWISEXOR";
		case Type::TK_BITWISENOT:			return "TK_BITWISENOT";
		case Type::TK_BITWISELEFTSHIFT:		return "TK_BITWISELEFTSHIFT";
		case Type::TK_BITWISERIGHTSHIFT:	return "TK_BITWISERIGHTSHIFT";
		case Type::TK_IF:					return "TK_IF";
		case Type::TK_ELSE:					return "TK_ELSE";
		case Type::TK_FOR:					return "TK_FOR";
		case Type::TK_WHILE:				return "TK_WHILE";
		case Type::TK_PRINT:				return "TK_PRINT";
		case Type::TK_LPAREN:				return "TK_LPAREN";
		case Type::TK_RPAREN:				return "TK_RPAREN";
		case Type::TK_LBRACE:				return "TK_LBRACE";
		case Type::TK_RBRACE:				return "TK_RBRACE";
		case Type::TK_LSQBRACKET:			return "TK_LSQBRACKET";
		case Type::TK_RSQBRACKET:			return "TK_RSQBRACKET";
		case Type::TK_SEMICOL:				return "TK_SEMICOL";
		case Type::TK_COMMA:				return "TK_COMMA";
		case Type::TK_IDENTIFIER:			return "TK_IDENTIFIER";
		case Type::TK_INTEGER:				return "TK_INTEGER";
		case Type::TK_FLOAT:				return "TK_FLOAT";
		case Type::TK_STRING:				return "TK_STRING";
		case Type::TK_CHARACTER:			return "TK_CHARACTER";
		case Type::TK_EOL:					return "TK_EOL";
		case Type::TK_EOI:					return "TK_EOI";
		case Type::TK_SINGLELINECOMMENT:	return "TK_SINGLELINECOMMENT";
		case Type::TK_MULTILINECOMMENT:		return "TK_MULTILINECOMMENT";
		case Type::TK_WHITESPACE:			return "TK_WHITESPACE";
		case Type::TK_BNFNONTERMINAL:		return "TK_BNFNONTERMINAL";
		case Type::TK_BNFASSIGNMENT:		return "TK_BNFASSIGNMENT";
		case Type::TK_BNFCODE:				return "TK_BNFCODE";
		case Type::TK_FUNCTIONCALL:			return "TK_FUNCTIONCALL";
		case Type::TK_PREFIXDECR:			return "TK_PREFIXDECR";
		case Type::TK_PREFIXINCR:			return "TK_PREFIXINCR";
		case Type::TK_POSTFIXDECR:			return "TK_POSTFIXDECR";
		case Type::TK_POSTFIXINCR:			return "TK_POSTFIXINCR";
		case Type::TK_SWITCH:				return "TK_SWITCH";
		case Type::TK_DEREF:				return "TK_DEREF";
		case Type::TK_DEREFARRAY:			return "TK_DEREFARRAY";
		case Type::TK_MEMBERACCESS:			return "TK_MEMBERACCESS";

		case Type::TK_UNKNOWN:				return "TK_UNKNOWN";
		}

		return "";
	}

	inline TokenType::Type fromString(std::string sTokenType)
	{
		if (sTokenType == "TK_MUL")				return Type::TK_MUL;
		else if (sTokenType == "TK_MULEQ")				return Type::TK_MULEQ;
		else if (sTokenType == "TK_DIV")				return Type::TK_DIV;
		else if (sTokenType == "TK_DIVEQ")				return Type::TK_DIVEQ;
		else if (sTokenType == "TK_MOD")				return Type::TK_MOD;
		else if (sTokenType == "TK_ADD")				return Type::TK_ADD;
		else if (sTokenType == "TK_ADDEQ")				return Type::TK_ADDEQ;
		else if (sTokenType == "TK_SUB")				return Type::TK_SUB;
		else if (sTokenType == "TK_SUBEQ")				return Type::TK_SUBEQ;
		else if (sTokenType == "TK_NEGATE")				return Type::TK_NEGATE;
		else if (sTokenType == "TK_NOT")				return Type::TK_NOT;
		else if (sTokenType == "TK_LT")					return Type::TK_LT;
		else if (sTokenType == "TK_LTEQ")				return Type::TK_LTEQ;
		else if (sTokenType == "TK_GT")					return Type::TK_GT;
		else if (sTokenType == "TK_GTEQ")				return Type::TK_GTEQ;
		else if (sTokenType == "TK_EQ")					return Type::TK_EQ;
		else if (sTokenType == "TK_NEQ")				return Type::TK_NEQ;
		else if (sTokenType == "TK_ASSIGN")				return Type::TK_ASSIGN;
		else if (sTokenType == "TK_LOGICALAND")			return Type::TK_LOGICALAND;
		else if (sTokenType == "TK_LOGICALOR")			return Type::TK_LOGICALOR;
		else if (sTokenType == "TK_BITWISEAND")			return Type::TK_BITWISEAND;
		else if (sTokenType == "TK_BITWISEOR")			return Type::TK_BITWISEOR;
		else if (sTokenType == "TK_BITWISEXOR")			return Type::TK_BITWISEXOR;
		else if (sTokenType == "TK_BITWISENOT")			return Type::TK_BITWISENOT;
		else if (sTokenType == "TK_BITWISELEFTSHIFT")	return Type::TK_BITWISELEFTSHIFT;
		else if (sTokenType == "TK_BITWISERIGHTSHIFT")	return Type::TK_BITWISERIGHTSHIFT;
		else if (sTokenType == "TK_IF")					return Type::TK_IF;
		else if (sTokenType == "TK_ELSE")				return Type::TK_ELSE;
		else if (sTokenType == "TK_FOR")				return Type::TK_FOR;
		else if (sTokenType == "TK_WHILE")				return Type::TK_WHILE;
		else if (sTokenType == "TK_PRINT")				return Type::TK_PRINT;
		else if (sTokenType == "TK_LPAREN")				return Type::TK_LPAREN;
		else if (sTokenType == "TK_RPAREN")				return Type::TK_RPAREN;
		else if (sTokenType == "TK_LBRACE")				return Type::TK_LBRACE;
		else if (sTokenType == "TK_RBRACE")				return Type::TK_RBRACE;
		else if (sTokenType == "TK_LSQBRACKET")			return Type::TK_LSQBRACKET;
		else if (sTokenType == "TK_RSQBRACKET")			return Type::TK_RSQBRACKET;
		else if (sTokenType == "TK_SEMICOL")			return Type::TK_SEMICOL;
		else if (sTokenType == "TK_COMMA")				return Type::TK_COMMA;
		else if (sTokenType == "TK_IDENTIFIER")			return Type::TK_IDENTIFIER;
		else if (sTokenType == "TK_INTEGER")			return Type::TK_INTEGER;
		else if (sTokenType == "TK_FLOAT")				return Type::TK_FLOAT;
		else if (sTokenType == "TK_STRING")				return Type::TK_STRING;
		else if (sTokenType == "TK_CHARACTER")			return Type::TK_CHARACTER;
		else if (sTokenType == "TK_EOL")				return Type::TK_EOL;
		else if (sTokenType == "TK_EOI")				return Type::TK_EOI;
		else if (sTokenType == "TK_SINGLELINECOMMENT")	return Type::TK_SINGLELINECOMMENT;
		else if (sTokenType == "TK_MULTILINECOMMENT")	return Type::TK_MULTILINECOMMENT;
		else if (sTokenType == "TK_WHITESPACE")			return Type::TK_WHITESPACE;
		else if (sTokenType == "TK_BNFNONTERMINAL")		return Type::TK_BNFNONTERMINAL;
		else if (sTokenType == "TK_BNFASSIGNMENT")		return Type::TK_BNFASSIGNMENT;
		else if (sTokenType == "TK_BNFCODE")			return Type::TK_BNFCODE;
		else if (sTokenType == "TK_FUNCTIONCALL")		return Type::TK_FUNCTIONCALL;
		else if (sTokenType == "TK_PREFIXDECR")			return Type::TK_PREFIXDECR;
		else if (sTokenType == "TK_PREFIXINCR")			return Type::TK_PREFIXINCR;
		else if (sTokenType == "TK_POSTFIXDECR")		return Type::TK_POSTFIXDECR;
		else if (sTokenType == "TK_POSTFIXINCR")		return Type::TK_POSTFIXINCR;
		else if (sTokenType == "TK_SWITCH")				return Type::TK_SWITCH;
		else if (sTokenType == "TK_DEREF")				return Type::TK_DEREF;
		else if (sTokenType == "TK_DEREFARRAY")			return Type::TK_DEREFARRAY;
		else if (sTokenType == "TK_MEMBERACCESS")		return Type::TK_MEMBERACCESS;

		else if (sTokenType == "TK_UNKNOWN")			return Type::TK_UNKNOWN;
		else											return Type::TK_INVALID;
	}
}

struct Token
{
	Token(TokenType::Type eTokenType, std::string sText, int iLine, int iColumn)
		: m_eTokenType(eTokenType)
		, m_sText(sText)
		, m_iLine(iLine)
		, m_iColumn(iColumn)
	{}

	TokenType::Type		m_eTokenType;
	std::string			m_sText;
	int					m_iLine;
	int					m_iColumn;

	TokenType::Type		getType() { return m_eTokenType; }
	const char*			getText() { return m_sText.c_str(); }
};

enum class OPCODE
{
	NOP = 0,
	FETCH,
	STORE,
	PUSH,
	POP,
	MUL,
	DIV,
	MOD,
	ADD,
	SUB,
	JMP_LT,
	JMP_LTEQ,
	JMP_GT,
	JMP_GTEQ,
	JMP_EQ,
	JMP_NEQ,
	LOGICALOR,
	LOGICALAND,
	BITWISEOR,
	BITWISEAND,
	BITWISEXOR,
	BITWISENOT,
	BITWISELEFTSHIFT,
	BITWISERIGHTSHIFT,
	_NOT,
	JMP,
	JZ,
	JNZ,
	PRTS,
	PRTC,
	PRTI,
	CALL,
	RET,
	SUB_REG,
	PUSHI,
	PUSHR,
	POPI,
	POPR,
	NEGATE,
	MALLOC,
	FREE,
	LDA,
	STA,
	CLR,
	VTBL,
	HLT,
};

enum class PRIMIIVETYPE
{
	INT_8,
	INT_16,
	INT_32,
	INT_64
};

struct CodeMap
{
	const char*		sOpCode;
	OPCODE			eOpCode;
	int32_t			iOpcodeOperandCount;
	PRIMIIVETYPE	ePRIMIIVETYPE;
};

enum class EREGISTERS
{
	RAX,	// Accumulator
	RCX,	// Counter
	RDX,	// Data
	RBX,	// Base
	RSP,	// Stack Pointer
	RBP,	// Stack Base Pointer
	RSI,	// Source
	RDI,	// Destination

	RMAX
};

struct RegisterMap
{
	const char*		sRegister;
	EREGISTERS		eEREGISTER;
};

typedef struct REGISTERS
{
	__int64		RAX;	// Accumulator
	__int64		RCX;	// Counter
	__int64		RDX;	// Data
	__int64		RBX;	// Base
	__int64		RSP;	// Stack Pointer
	__int64		RBP;	// Stack Base Pointer
	__int64		RSI;	// Source
	__int64		RDI;	// Destination

	__int32		EFLAGS;
	__int32		EIP;
};


enum class ENUM_OP_PRECEDENCE
{
	OP_INVALID = 0,

	OP_LPAREN,

	OP_LOGICALOR,
	OP_LOGICALAND,

	OP_BITWISEOR,
	OP_BITWISEAND,
	OP_BITWISEXOR,
	OP_BITWISELEFTSHIFT,
	OP_BITWISERIGHTSHIFT,

	OP_EQ,
	OP_NEQ = OP_EQ,

	OP_LT,
	OP_LTEQ = OP_LT,
	OP_GT = OP_LT,
	OP_GTEQ = OP_LT,

	OP_ADD,
	OP_SUB = OP_ADD,

	OP_MUL,
	OP_DIV = OP_MUL,
	OP_MOD = OP_MUL,

	OP_NOT,
	OP_PLUS = OP_NOT,
	OP_NEGATE = OP_NOT,
	OP_BITWISENOT = OP_NOT,

	OP_RPAREN,
};

enum class ASTNodeType
{
	ASTNode_INVALID = -1,
	ASTNode_ROOT,

	ASTNode_MUL,
	ASTNode_MULEQ,
	ASTNode_DIV,
	ASTNode_DIVEQ,
	ASTNode_MOD,

	ASTNode_ADD,
	ASTNode_ADDEQ,
	ASTNode_SUB,
	ASTNode_SUBEQ,

	ASTNode_NEGATE,
	ASTNode_NOT,
	ASTNode_DEREF,
	ASTNode_DEREFARRAY,

	ASTNode_LT,
	ASTNode_LTEQ,
	ASTNode_GT,
	ASTNode_GTEQ,
	ASTNode_EQ,
	ASTNode_NEQ,

	ASTNode_ASSIGN,

	ASTNode_LOGICALAND,
	ASTNode_LOGICALOR,
	ASTNode_BITWISEAND,
	ASTNode_BITWISEOR,
	ASTNode_BITWISEXOR,
	ASTNode_BITWISENOT,
	ASTNode_BITWISELEFTSHIFT,
	ASTNode_BITWISERIGHTSHIFT,

	ASTNode_IF,
	ASTNode_ELSE,
	ASTNode_FOR,
	ASTNode_WHILE,

	ASTNode_PRINT,
	ASTNode_PUTC,

	ASTNode_IDENTIFIER,
	ASTNode_INTEGER,
	ASTNode_FLOAT,
	ASTNode_STRING,
	ASTNode_CHARACTER,
	ASTNode_STATEMENT,
	ASTNode_EXPRESSION,

	ASTNode_FUNCTIONDEF,
	ASTNode_FUNCTIONRETURNTYPE,
	ASTNode_FUNCTIONARGLIST,
	ASTNode_FUNCTIONSTART,
	ASTNode_FUNCTIONEND,
	ASTNode_FUNCTIONCALL,
	ASTNode_RETURNSTMT,
	ASTNode_FUNCTIONCALLEND,
	ASTNode_EXPRESSION_PREFIX,
	ASTNode_EXPRESSION_POSTFIX,
	ASTNode_PREDECR,
	ASTNode_PREINCR,
	ASTNode_POSTDECR,
	ASTNode_POSTINCR,
	ASTNode_SWITCH,
	ASTNode_SWITCHCASE,
	ASTNode_SWITCHDEFAULT,
	ASTNode_SWITCHBREAK,
	ASTNode_MALLOC,
	ASTNode_FREE,
	ASTNode_TYPE,
	ASTNode_TYPESTATIC,
	ASTNode_TYPEARRAY,
	ASTNode_TYPEARRAYELEMENTS,
	ASTNode_STRUCTDEF,
	ASTNode_STRUCTSTART,
	ASTNode_STRUCTEND,
	ASTNode_TYPESTRUCT,
	ASTNode_CONSTRUCTORCALL,
	ASTNode_CONSTRUCTORCALLEND,
	ASTNode_MEMBERACCESS,
};

typedef struct Tree
{
	Tree()
		: m_pParentNode(nullptr)
		, m_pLeftNode(nullptr)
		, m_pRightNode(nullptr)
		, m_eASTNodeType(ASTNodeType::ASTNode_INVALID)
		, m_sAdditionalInfo("")
		, m_bIsPointerType(false)
	{
		// Reset the "variablePos" info to "-1"
		SET_INFO_FOR_KEY(this, "variablePos", "-1");
	}

	void addChild(Tree* pNode)
	{
		if (pNode != nullptr)
		{
			m_vStatements.push_back(pNode);
			pNode->m_pParentNode = this;
		}
	}

	void insertAt(int i, Tree* pNode)
	{
		if (pNode != nullptr)
		{
			if (i < m_vStatements.size())
			{
				m_vStatements.insert(m_vStatements.begin() + i, pNode);
			}
			else
				m_vStatements.push_back(pNode);

			pNode->m_pParentNode = this;
		}
	}

	Tree* getLastStatement()
	{
		return m_vStatements.back();
	}

	void removeFromParent()
	{
		int iCount = 0;
		for (Tree* pChild : m_pParentNode->m_vStatements)
		{
			if (pChild == this)
			{
				m_pParentNode->m_vStatements.erase(m_pParentNode->m_vStatements.begin() + iCount);
				m_pParentNode = nullptr;
				break;
			}

			iCount++;
		}
	}

	void setAdditionalInfo(std::string sKey, std::string sValue)
	{
		std::map<std::string, std::string>::const_iterator itr = m_MapAdditionalInfo.find(sKey);
		if (itr != m_MapAdditionalInfo.end())
		{
			m_MapAdditionalInfo[sKey] = sValue;
		}
		else
		{
			m_MapAdditionalInfo.insert(std::pair<std::string, std::string>(sKey, sValue));
		}
	}

	std::string getAdditionalInfoFor(std::string sKey)
	{
		std::string sReturn = "";
		std::map<std::string, std::string>::const_iterator itr = m_MapAdditionalInfo.find(sKey);
		if (itr != m_MapAdditionalInfo.end())
		{
			sReturn = itr->second;
		}

		return sReturn;
	}

	ASTNodeType			m_eASTNodeType;

	std::vector<Tree*>	m_vStatements;
	std::string			m_sAdditionalInfo;

	bool				m_bIsPointerType;

	Tree*				m_pParentNode;
	Tree*				m_pLeftNode;
	Tree*				m_pRightNode;

	std::map<std::string, std::string>	m_MapAdditionalInfo;
} Tree;

enum class E_VARIABLESCOPE
{
	INVALID = -1,
	ARGUMENT,
	LOCAL,
	STATIC,
	MEMBER
};

static std::string toString(E_VARIABLESCOPE eE_VARIABLESCOPE)
{
	std::string sE_VARIABLESCOPE = "";
	switch (eE_VARIABLESCOPE)
	{
	case E_VARIABLESCOPE::INVALID:
		sE_VARIABLESCOPE = "INVALID";
		break;
	case E_VARIABLESCOPE::ARGUMENT:
		sE_VARIABLESCOPE = "ARGUMENT";
		break;
	case E_VARIABLESCOPE::LOCAL:
		sE_VARIABLESCOPE = "LOCAL";
		break;
	case E_VARIABLESCOPE::STATIC:
		sE_VARIABLESCOPE = "STATIC";
		break;
	case E_VARIABLESCOPE::MEMBER:
		sE_VARIABLESCOPE = "MEMBER";
		break;
	}

	return sE_VARIABLESCOPE;
}

static E_VARIABLESCOPE toScope(std::string sE_VARIABLESCOPE)
{
	if (sE_VARIABLESCOPE == "INVALID") return E_VARIABLESCOPE::INVALID;
	else if (sE_VARIABLESCOPE == "ARGUMENT") return E_VARIABLESCOPE::ARGUMENT;
	else if (sE_VARIABLESCOPE == "LOCAL") return E_VARIABLESCOPE::LOCAL;
	else if (sE_VARIABLESCOPE == "STATIC") return E_VARIABLESCOPE::STATIC;
	else if (sE_VARIABLESCOPE == "MEMBER") return E_VARIABLESCOPE::MEMBER;
}

static std::map<std::string, Tree*>		m_MapVariableToASTNodeCache;

typedef struct StructInfo
{
	StructInfo(Tree* pNode)
	: m_pNode(pNode)
	, m_bHasConstructor(false)
	, m_bHasDestructor(false)
	, m_ParentStructInfo(nullptr)
	, m_iVirtualFunctionCount(-1)
	, m_iSizeOf(-1)
	, m_iVTableOffset(-1)
	, m_bHasVTable(false)
	{
		m_sStructName = GET_INFO_FOR_KEY(pNode, "text");
		scanStructForMemberVariables(pNode);
	}

	int32_t sizeOfMe()					// sizeOf 'this'
	{
		int32_t iSizeOf = (sizeof(int32_t) * m_vMemberVariables.size());
		iSizeOf += (sizeof(int32_t) * (m_bHasVTable ? 1 : 0)); // VTABLE pointer

		return iSizeOf;
	}

	int32_t sizeOf()					// sizeOf 'this' + sizeOf 'parent'
	{
		if (m_iSizeOf < 0)
		{
			m_iSizeOf = sizeOfMe();

			if (m_ParentStructInfo != nullptr)
			{
				m_iSizeOf += m_ParentStructInfo->sizeOf();
			}
		}

		return m_iSizeOf;
	}

	void setParentStruct(StructInfo* pParentStructInfo)
	{
		m_ParentStructInfo = pParentStructInfo;
	}

	void scanStructForMemberVariables(Tree* pNode)
	{
		for (Tree* pChild : pNode->m_vStatements)
		{
			switch (pChild->m_eASTNodeType)
			{
				case ASTNodeType::ASTNode_TYPE:
				case ASTNodeType::ASTNode_TYPEARRAY:
				{
					m_vMemberVariables.push_back(pChild);
				}
				break;
			}
		}
	}

	void addFunction(void* vpFunctionInfo)
	{
		if (vpFunctionInfo != nullptr)
		{
			m_vMemberFunctions.push_back(vpFunctionInfo);
		}
	}

	bool hasFunctionByName(std::string sFunctionName)
	{
		bool bReturn = false;
		std::vector<void*>::const_iterator pFunctionItr = m_vMemberFunctions.cbegin();
		for (; pFunctionItr != m_vMemberFunctions.cend(); ++pFunctionItr)
		{
			Tree* pFunctionNode = (Tree*)*pFunctionItr;
			if (GET_INFO_FOR_KEY(pFunctionNode, "text") == sFunctionName)
			{
				bReturn = true;
				break;
			}
		}

		return bReturn;
	}

	int32_t structOffsetToVariable(const char* sVariableName)
	{
		int32_t iOffset = 0;
		bool bFound = false;
		for (Tree* pMemberVar : m_vMemberVariables)
		{
			if (GET_INFO_FOR_KEY(pMemberVar, "givenName") == sVariableName)
			{
				bFound = true;
				break;
			}
		}

		if (NOT bFound && m_ParentStructInfo != nullptr)
		{
			iOffset += sizeOfMe();
			iOffset += m_ParentStructInfo->structOffsetToVariable(sVariableName);
		}

		return iOffset;
	}

	Tree* getMemberVariableASTNode(const char* sVariableName)
	{
		Tree* pASTNode = nullptr;
		std::map<std::string, Tree*>::const_iterator itr = m_MapVariableToASTNodeCache.find(sVariableName);
		if (itr != m_MapVariableToASTNodeCache.end())
		{
			pASTNode = m_MapVariableToASTNodeCache[sVariableName];
		}
		else
		{
			short iShortPosition = 0;
			for (Tree* pMemberVar : m_vMemberVariables) // Starts with index 0
			{
				if (GET_INFO_FOR_KEY(pMemberVar, "givenName") == sVariableName)
				{
					pASTNode = pMemberVar;
					break;
				}

				iShortPosition++;
			}

			if (pASTNode == nullptr && m_ParentStructInfo != nullptr)
			{
				pASTNode = m_ParentStructInfo->getMemberVariableASTNode(sVariableName);
			}

			assert(pASTNode != nullptr);
			if (pASTNode != nullptr)
			{
				// While scanning for the variable, find its position in the respective block & set it.
				std::string sVariablePos = GET_INFO_FOR_KEY(pASTNode, "variablePos");
				if (sVariablePos == "-1")
				{
					int32_t iPositionOperand = 0;
					E_VARIABLESCOPE eVARIABLESCOPE = E_VARIABLESCOPE::INVALID;
					eVARIABLESCOPE = toScope(GET_INFO_FOR_KEY(pASTNode, "scope"));
					assert(eVARIABLESCOPE != E_VARIABLESCOPE::INVALID);

					if (eVARIABLESCOPE == E_VARIABLESCOPE::MEMBER && m_bHasVTable)
						iShortPosition++;

					iPositionOperand = (int32_t)eVARIABLESCOPE;
					iPositionOperand <<= sizeof(int16_t) * 8;
					iPositionOperand |= (iShortPosition & 0x0000FFFF);

					char sVariablePosFound[255] = { 0 };
					_itoa(iPositionOperand, sVariablePosFound, 10);
					SET_INFO_FOR_KEY(pASTNode, "variablePos", sVariablePosFound);
				}

				m_MapVariableToASTNodeCache[sVariableName] = pASTNode;
			}
		}

		return pASTNode;
	}

	int32_t getMemberVariablePosition(const char* sLocalVariableName)
	{
		int32_t iPositionOperand = 0;

		Tree* pASTNode = getMemberVariableASTNode(sLocalVariableName);
		assert(pASTNode != nullptr);
		if (pASTNode != nullptr)
		{
			std::string sVariablePos = GET_INFO_FOR_KEY(pASTNode, "variablePos");
			iPositionOperand = atoi(sVariablePos.c_str());
		}

		return iPositionOperand;
	}

	int32_t getVTableOffset()
	{
		return m_iVTableOffset;
	}

	Tree*									m_pNode;
	std::string								m_sStructName;
	std::vector<void*>						m_vMemberFunctions;
	std::vector<Tree*>						m_vMemberVariables;
	std::vector<void*>						m_vVirtualFunctionTable;
	bool									m_bHasConstructor;
	bool									m_bHasDestructor;

	int32_t									m_iVirtualFunctionCount;
	StructInfo*								m_ParentStructInfo;

	int32_t									m_iSizeOf;
	int32_t									m_iVTableOffset;
	bool									m_bHasVTable;
} StructInfo;

typedef struct FunctionInfo
{
	FunctionInfo(Tree* pNode, int iOffset)
		: m_pNode(pNode)
		, m_iStartOffsetInCode(iOffset)
		, m_sFunctionName(GET_INFO_FOR_KEY(pNode, "text"))
		, m_pFunctionReturnType(pNode->m_pLeftNode)
		, m_pFunctionArguments(pNode->m_pRightNode)
		, m_pParentStructInfo(nullptr)
	{
		scanFunctionForLocals(pNode);
		scanFunctionForArguments(pNode);
	}

	void scanFunctionForLocals(Tree* pNode)
	{
		for (Tree* pChild : pNode->m_vStatements)
		{
			switch (pChild->m_eASTNodeType)
			{
			case ASTNodeType::ASTNode_TYPE:
			case ASTNodeType::ASTNode_TYPEARRAY:
			case ASTNodeType::ASTNode_TYPESTRUCT:
			{
				m_vLocalVariables.push_back(pChild);
			}
			break;
			case ASTNodeType::ASTNode_IF:
			{
				scanFunctionForLocals(pChild);
				Tree* pElseNode = pChild->m_pRightNode;
				if (pElseNode != nullptr)
					scanFunctionForLocals(pElseNode);
			}
			break;
			case ASTNodeType::ASTNode_WHILE:
			{
				scanFunctionForLocals(pChild);
			}
			break;
			}
		}
	}

	void scanFunctionForArguments(Tree* pNode)
	{
		for (Tree* pChild : m_pFunctionArguments->m_vStatements)
		{
			switch (pChild->m_eASTNodeType)
			{
			case ASTNodeType::ASTNode_TYPE:
			{
				m_vArguments.push_back(pChild);
			}
			break;
			}
		}
	}

	Tree* getVariableASTNode(const char* sVariableName)
	{
		Tree* pASTNode = nullptr;
		std::map<std::string, Tree*>::const_iterator itr = m_MapVariableToASTNodeCache.find(sVariableName);
		if (itr != m_MapVariableToASTNodeCache.end())
		{
			pASTNode = m_MapVariableToASTNodeCache[sVariableName];
		}
		else
		{
			short iShortPosition = 0;
			bool bFound = false;

			// Check for 'Locals'
			for (Tree* pLocalVar : m_vLocalVariables) // Starts with index 1
			{
				iShortPosition++;
				if (GET_INFO_FOR_KEY(pLocalVar, "text") == sVariableName)
				{
					pASTNode = pLocalVar;
					bFound = true;
					break;
				}
			}

			// Check for 'Arguments'
			if (NOT bFound)
			{
				iShortPosition = 0;
				for (Tree* pArgVar : m_vArguments) // Starts with index 0
				{
					if (GET_INFO_FOR_KEY(pArgVar, "text") == sVariableName)
					{
						pASTNode = pArgVar;
						bFound = true;
						break;
					}

					iShortPosition++;
				}
			}

			// Check for 'Member Variables'
			if (NOT bFound && m_pParentStructInfo != nullptr)
			{
				iShortPosition = 0;
				for (Tree* pMemberVar : m_pParentStructInfo->m_vMemberVariables) // Starts with index 0
				{
					if (GET_INFO_FOR_KEY(pMemberVar, "text") == sVariableName)
					{
						pASTNode = pMemberVar;
						bFound = true;
						break;
					}

					iShortPosition++;
				}
			}

			// Check for 'Static'
			if (NOT bFound)
			{
				iShortPosition = 0;
				for (Tree* pStaticVar : m_vStaticVariables) // Starts with index 0
				{
					if (GET_INFO_FOR_KEY(pStaticVar, "text") == sVariableName)
					{
						pASTNode = pStaticVar;
						break;
					}

					iShortPosition++;
				}
			}

			assert(pASTNode != nullptr);
			if (pASTNode != nullptr)
			{
				// While scanning for the variable, find its position in the respective block & set it.
				std::string sVariablePos = GET_INFO_FOR_KEY(pASTNode, "variablePos");
				if (sVariablePos == "-1")
				{
					int32_t iPositionOperand = 0;
					E_VARIABLESCOPE eVARIABLESCOPE = E_VARIABLESCOPE::INVALID;
					eVARIABLESCOPE = toScope(GET_INFO_FOR_KEY(pASTNode, "scope"));
					assert(eVARIABLESCOPE != E_VARIABLESCOPE::INVALID);

					if (eVARIABLESCOPE == E_VARIABLESCOPE::MEMBER && m_pParentStructInfo->m_bHasVTable)
						iShortPosition++;

					iPositionOperand = (int32_t)eVARIABLESCOPE;
					iPositionOperand <<= sizeof(int16_t) * 8;
					iPositionOperand |= (iShortPosition & 0x0000FFFF);

					char sVariablePosFound[255] = { 0 };
					_itoa(iPositionOperand, sVariablePosFound, 10);
					SET_INFO_FOR_KEY(pASTNode, "variablePos", sVariablePosFound);
				}

				m_MapVariableToASTNodeCache[sVariableName] = pASTNode;
			}
		}

		return pASTNode;
	}

	int32_t getLocalVariablePosition(const char* sLocalVariableName)
	{
		int32_t iPositionOperand = 0;

		Tree* pASTNode = getVariableASTNode(sLocalVariableName);
		assert(pASTNode != nullptr);
		if (pASTNode != nullptr)
		{
			std::string sVariablePos = GET_INFO_FOR_KEY(pASTNode, "variablePos");
			iPositionOperand = atoi(sVariablePos.c_str());
		}

		return iPositionOperand;
	}

	std::string getVariableNodeType(const char* sLocalVariableName)
	{
		std::string sType;

		Tree* pVariableASTNode = getVariableASTNode(sLocalVariableName);
		assert(pVariableASTNode != nullptr);
		if (pVariableASTNode != nullptr)
		{
			sType = GET_INFO_FOR_KEY(pVariableASTNode, "type");
		}

		return sType;
	}

	bool IsVariableAPointerType(const char* sLocalVariableName)
	{
		bool bIsPointerType = false;

		Tree* pVariableASTNode = getVariableASTNode(sLocalVariableName);
		assert(pVariableASTNode != nullptr);
		if (pVariableASTNode != nullptr)
		{
			bIsPointerType = pVariableASTNode->m_bIsPointerType;
		}

		return bIsPointerType;
	}

	int getLocalVariableCount()
	{
		return m_vLocalVariables.size();
	}

	int getArgumentsCount()
	{
		return m_vArguments.size();
	}

	static void addStaticVariable(Tree* pNode)
	{
		bool bIsNew = true;
		for (Tree* pStaticVar : m_vStaticVariables)
		{
			if (GET_INFO_FOR_KEY(pNode, "text") == GET_INFO_FOR_KEY(pStaticVar, "text")) {
				bIsNew = false;
				break;
			}
		}

		if (bIsNew)
			m_vStaticVariables.push_back(pNode);
	}

	void setParent(StructInfo* pStructInfo)
	{
		m_pParentStructInfo = pStructInfo;

		// Check for "virtual"
		{
			std::string sVirtual = GET_INFO_FOR_KEY(m_pNode, "isVirtual");
			if (NOT pStructInfo->m_bHasVTable && sVirtual == "virtual")
			{
				pStructInfo->m_bHasVTable = true;
			}
		}
	}

	Tree*							m_pNode;
	int								m_iStartOffsetInCode;
	std::string						m_sFunctionName;

	Tree*							m_pFunctionReturnType;
	Tree*							m_pFunctionArguments;

	std::vector<Tree*>				m_vLocalVariables;
	std::vector<Tree*>				m_vArguments;

	static std::vector<Tree*>		m_vStaticVariables;
	StructInfo*						m_pParentStructInfo;
} FunctionInfo;

static int32_t calculateVirtualFunctionCount(StructInfo* pStructInfo)
{
	if (pStructInfo->m_iVirtualFunctionCount < 0)
	{
		pStructInfo->m_iVirtualFunctionCount = 0;
		std::vector<void*>::const_iterator itrFunc = pStructInfo->m_vMemberFunctions.begin();
		for (; itrFunc != pStructInfo->m_vMemberFunctions.end(); ++itrFunc)
		{
			FunctionInfo* pFunctionInfo = (FunctionInfo*)*itrFunc;
			if (pFunctionInfo != nullptr)
			{
				Tree* pNode = pFunctionInfo->m_pNode;
				std::string sVirtual = GET_INFO_FOR_KEY(pNode, "isVirtual");
				if (NOT sVirtual.empty())
				{
					pStructInfo->m_iVirtualFunctionCount++;
				}
			}
		}
	}

	return pStructInfo->m_iVirtualFunctionCount;
}
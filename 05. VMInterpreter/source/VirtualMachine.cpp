#include "VirtualMachine.h"
#include "RandomAccessFile.h"
#include <assert.h>
#include <algorithm>
#include <iostream>
#include "ConsoleColor.h"
#include "meta/MetaFunction.h"

#define VERBOSE	1

VirtualMachine*	VirtualMachine::m_pVMInstance = nullptr;

enum class PRIMIIVETYPE
{
	INT_8,
	INT_16,
	INT_32,
	INT_64
};

enum class E_VARIABLESCOPE
{
	INVALID = -1,
	ARGUMENT,
	LOCAL,
	STATIC,
	MEMBER
};

enum class E_FUNCTIONCALLTYPE
{
	INVALID = -1,
	NORMAL,
	VIRTUAL,
};

struct CodeMap
{
	const char*		sOpCode;
	OPCODE			eOpCode;
	int				iOpcodeOperandCount;
	PRIMIIVETYPE	ePRIMIIVETYPE;
} opCodeMap[] =
{
	{ "NOP",		OPCODE::NOP,		1,  PRIMIIVETYPE::INT_8 },
	{ "FETCH",		OPCODE::FETCH,		2,  PRIMIIVETYPE::INT_32 },
	{ "STORE",		OPCODE::STORE,		2,  PRIMIIVETYPE::INT_32 },
	{ "PUSH",		OPCODE::PUSH,		2,  PRIMIIVETYPE::INT_32 },
	{ "POP",		OPCODE::POP,		2,  PRIMIIVETYPE::INT_32 },
	{ "MUL",		OPCODE::MUL,		1,  PRIMIIVETYPE::INT_8 },
	{ "DIV",		OPCODE::DIV,		1,  PRIMIIVETYPE::INT_8 },
	{ "MOD",		OPCODE::MOD,		1,  PRIMIIVETYPE::INT_8 },
	{ "ADD",		OPCODE::ADD,		1,  PRIMIIVETYPE::INT_8 },
	{ "SUB",		OPCODE::SUB,		1,  PRIMIIVETYPE::INT_8 },
	{ "JMP_LT",		OPCODE::JMP_LT,		1,  PRIMIIVETYPE::INT_8 },
	{ "JMP_LTEQ",	OPCODE::JMP_LTEQ,	1,  PRIMIIVETYPE::INT_8 },
	{ "JMP_GT",		OPCODE::JMP_GT,		1,  PRIMIIVETYPE::INT_8 },
	{ "JMP_GTEQ",	OPCODE::JMP_GTEQ,	1,  PRIMIIVETYPE::INT_8 },
	{ "JMP_EQ",		OPCODE::JMP_EQ,		1,  PRIMIIVETYPE::INT_8 },
	{ "JMP_NEQ",	OPCODE::JMP_NEQ,	1,  PRIMIIVETYPE::INT_8 },
	{ "LOGICALOR",	OPCODE::LOGICALOR,	1,  PRIMIIVETYPE::INT_8 },
	{ "LOGICALAND",	OPCODE::LOGICALAND,	1,  PRIMIIVETYPE::INT_8 },
	{ "BITWISEOR",	OPCODE::BITWISEOR,	1,  PRIMIIVETYPE::INT_8 },
	{ "BITWISEAND",	OPCODE::BITWISEAND,	1,  PRIMIIVETYPE::INT_8 },
	{ "BITWISEXOR",	OPCODE::BITWISEXOR,	1,  PRIMIIVETYPE::INT_8 },
	{ "BITWISENOT",	OPCODE::BITWISENOT,	1,  PRIMIIVETYPE::INT_8 },
	{ "BITWISELEFTSHIFT",	OPCODE::BITWISELEFTSHIFT,	1,  PRIMIIVETYPE::INT_8 },
	{ "BITWISERIGHTSHIFT",	OPCODE::BITWISERIGHTSHIFT,	1,  PRIMIIVETYPE::INT_8 },
	{ "NOT",		OPCODE::_NOT,		1,  PRIMIIVETYPE::INT_8 },
	{ "JMP",		OPCODE::JMP,		2,  PRIMIIVETYPE::INT_32 },
	{ "JZ",			OPCODE::JZ,			2,  PRIMIIVETYPE::INT_32 },
	{ "JNZ",		OPCODE::JNZ,		2,  PRIMIIVETYPE::INT_32 },
	{ "PRTS",		OPCODE::PRTS,		1,  PRIMIIVETYPE::INT_8 },
	{ "PRTC",		OPCODE::PRTC,		1,  PRIMIIVETYPE::INT_8 },
	{ "PRTI",		OPCODE::PRTI,		1,  PRIMIIVETYPE::INT_8 },
	{ "CALL",		OPCODE::CALL,		2,  PRIMIIVETYPE::INT_32 },
	{ "RET",		OPCODE::RET,		1,  PRIMIIVETYPE::INT_8 },
	{ "SUB_REG",	OPCODE::SUB_REG,	3,  PRIMIIVETYPE::INT_8 },

	{ "PUSHI",		OPCODE::PUSHI,		2,  PRIMIIVETYPE::INT_32 },
	{ "PUSHR",		OPCODE::PUSHR,		2,  PRIMIIVETYPE::INT_8 },
	{ "POPI",		OPCODE::POPI,		2,  PRIMIIVETYPE::INT_8 },
	{ "POPR",		OPCODE::POPR,		2,  PRIMIIVETYPE::INT_8 },
	{ "NEGATE",		OPCODE::NEGATE,		1,  PRIMIIVETYPE::INT_8 },

	{ "MALLOC",		OPCODE::MALLOC,		1,  PRIMIIVETYPE::INT_8 },
	{ "FREE",		OPCODE::FREE,		2,  PRIMIIVETYPE::INT_32 },

	{ "LDA",		OPCODE::LDA,		2,  PRIMIIVETYPE::INT_32},
	{ "STA",		OPCODE::STA,		2,  PRIMIIVETYPE::INT_32},

	{ "CLR",		OPCODE::CLR,		7,  PRIMIIVETYPE::INT_32},

	{ "VTBL",		OPCODE::VTBL,		2,  PRIMIIVETYPE::INT_32 },

	{ "MEMSET",		OPCODE::MEMSET,		1,  PRIMIIVETYPE::INT_32 },
	{ "MEMCPY",		OPCODE::MEMCPY,		1,  PRIMIIVETYPE::INT_32 },
	{ "MEMCMP",		OPCODE::MEMCMP,		1,  PRIMIIVETYPE::INT_32 },
	{ "MEMCHR",		OPCODE::MEMCHR,		1,  PRIMIIVETYPE::INT_32 },
	{ "SYSCALL",	OPCODE::SYSCALL,	2,  PRIMIIVETYPE::INT_32 },
	{ "PUSHF",		OPCODE::PUSHF,		2,  PRIMIIVETYPE::INT_32 },

	{ "MULF",		OPCODE::MULF,		1,  PRIMIIVETYPE::INT_8 },
	{ "DIVF",		OPCODE::DIVF,		1,  PRIMIIVETYPE::INT_8 },
	{ "ADDF",		OPCODE::ADDF,		1,  PRIMIIVETYPE::INT_8 },
	{ "SUBF",		OPCODE::SUBF,		1,  PRIMIIVETYPE::INT_8 },

	{ "PRTF",		OPCODE::PRTF,		1,  PRIMIIVETYPE::INT_8 },

	{ "HLT",		OPCODE::HLT,		1,  PRIMIIVETYPE::INT_8 },
};

VirtualMachine::VirtualMachine()
{ }

VirtualMachine::~VirtualMachine()
{

}

VirtualMachine*	VirtualMachine::create()
{
	if (m_pVMInstance == nullptr)
	{
		m_pVMInstance = new VirtualMachine();
	}

	return m_pVMInstance;
}

void VirtualMachine::run(const char* sMachineCodeFile)
{
	if (sMachineCodeFile != nullptr && strlen(sMachineCodeFile) > 0)
	{
		RandomAccessFile* pRaf = new RandomAccessFile();
		bool bCanRead = pRaf->openForRead(sMachineCodeFile);
		if (bCanRead)
		{
			unsigned long iLength = pRaf->length();
			char* sBuff = new char[iLength + 1];
			memset(sBuff, 0, iLength);

			unsigned long iBytesRead = pRaf->read(sBuff);
			if (iBytesRead > 0)
			{
				reset();
				load(sBuff, iLength);
				execute(sBuff);
			}

			delete[] sBuff;
			sBuff = nullptr;
		}
		pRaf->close();
	}
}

void VirtualMachine::reset()
{
	memset(&REGS, 0, sizeof(REGS));
	memset(&RAM, 0, sizeof(char) * MAX_RAM_SIZE);

	REGS.EIP = 0;
	REGS.RSP = 0;
	REGS.RBP = 0;

	REGS.CS = CS_START_OFFSET;
	REGS.SS = SS_START_OFFSET;
	REGS.DS = DS_START_OFFSET;

	m_vUnAllocatedList.push_back(HeapNode(0, MAX_HEAP_SIZE));

	m_bRunning = false;
}

int VirtualMachine::loadBSS(const char* iByteCode, int startOffset, int iBuffLength)
{
	int iOffset = startOffset;
	int iStringCount = *((int16_t*)&iByteCode[iOffset]);
	iOffset += sizeof(short);

	int iStringSize = 0;
	int iStringStartOffset = DS_START_OFFSET + (iStringCount * sizeof(int32_t));
	int32_t* pStringLocOffset = (int32_t*)DATA;

	// Load Strings in Memory
	for (int i = 0; i < iStringCount; i++)
	{
		iStringSize = iByteCode[iOffset++];

		*pStringLocOffset++ = iStringStartOffset;

		memcpy(&RAM[iStringStartOffset], iByteCode + iOffset, sizeof(char) * iStringSize);
		RAM[iStringStartOffset + sizeof(char) * iStringSize + 1] = 0;

		iStringStartOffset += iStringSize + 1;
		iOffset += iStringSize;
	}

	// Get Static variable count
	{
		int32_t iStaticVariableCount = (*(int32_t*)&iByteCode[iOffset]);

		GLOBALS = (int32_t*)&RAM[iStringStartOffset];
		iOffset += sizeof(int32_t);
		iStringStartOffset += sizeof(int32_t) * iStaticVariableCount;
	}

	HEAP = &RAM[iStringStartOffset];
	REGS.GS = iStringStartOffset;

	return iOffset;
}

int VirtualMachine::loadCode(const char* iByteCode, int startOffset, int iBuffLength)
{
	int iOffset = startOffset;
	int iCodeSize = iBuffLength - iOffset;

	memcpy(CODE, iByteCode + iOffset, sizeof(char) * iCodeSize);

	iOffset += iCodeSize;

	return iOffset;
}

void VirtualMachine::load(const char* iByteCode, int iBuffLength)
{
	CODE = (int8_t*)&RAM[CS_START_OFFSET];
	STACK = (int32_t*)&RAM[SS_START_OFFSET];
	DATA = (int8_t*)&RAM[DS_START_OFFSET];

	int iEndOffset = 0;
	iEndOffset = loadBSS(iByteCode, 0, iBuffLength);
	iEndOffset = loadCode(iByteCode, iEndOffset, iBuffLength);

	scriptTest();
}

void VirtualMachine::execute(const char* iByteCode)
{
	m_bRunning = true;
	while (m_bRunning)
	{
		eval(fetch());
	}
}

OPCODE VirtualMachine::fetch()
{
	return (OPCODE)CODE[REGS.EIP++];
}

void VirtualMachine::eval(OPCODE eOpCode)
{
	int32_t iOperand = 0, iTemp1 = 0, iTemp2 = 0;
	float fTemp1 = 0.0f, fTemp2 = 0.0f;
	switch (eOpCode)
	{
	case OPCODE::FETCH:
	{
		int32_t iVariable = READ_OPERAND(eOpCode);
		{
			int32_t iValue = getValueIn(iVariable);
			STACK[--REGS.RSP] = iValue;
		}
	}
	break;
	case OPCODE::STORE:
	{
		store(eOpCode);
	}
	break;
	case OPCODE::PUSH:
	case OPCODE::PUSHI:
		iOperand = READ_OPERAND(eOpCode);
		STACK[--REGS.RSP] = iOperand;
		break;
	case OPCODE::PUSHF:
		iOperand = READ_OPERAND(eOpCode);
		STACK[--REGS.RSP] = iOperand;
		break;
	case OPCODE::PUSHR:
		pushr(eOpCode);
		break;
	case OPCODE::POP:
	case OPCODE::POPI:
		break;
	case OPCODE::POPR:
		popr(eOpCode);
		break;
	case OPCODE::CALL:
	{
		call(eOpCode);
	}
	break;
	case OPCODE::SYSCALL:
	{
		sysCall(eOpCode);
	}
	break;
	case OPCODE::RET:
		REGS.EIP = STACK[REGS.RSP++];			// Pop the Return address off the stack.
	break;
	case OPCODE::SUB_REG:
		subreg(eOpCode);
	break;
	case OPCODE::MUL:
	{
		iTemp1 = STACK[REGS.RSP++];
		iTemp2 = STACK[REGS.RSP++];
		iTemp1 *= iTemp2;

		STACK[--REGS.RSP] = iTemp1;
	}
	break;
	case OPCODE::MULF:
	{
		memcpy_s(&fTemp1, sizeof(float), &STACK[REGS.RSP++], sizeof(float));
		memcpy_s(&fTemp2, sizeof(float), &STACK[REGS.RSP++], sizeof(float));

		fTemp1 *= fTemp2;

		memcpy_s(&STACK[--REGS.RSP], sizeof(float), &fTemp1, sizeof(float));
	}
	break;
	case OPCODE::DIV:
		iTemp1 = STACK[REGS.RSP++];
		iTemp2 = STACK[REGS.RSP++];
		iTemp2 /= iTemp1;

		STACK[--REGS.RSP] = iTemp2;
	break;
	case OPCODE::DIVF:
	{
		memcpy_s(&fTemp1, sizeof(float), &STACK[REGS.RSP++], sizeof(float));
		memcpy_s(&fTemp2, sizeof(float), &STACK[REGS.RSP++], sizeof(float));

		fTemp2 /= fTemp1;

		memcpy_s(&STACK[--REGS.RSP], sizeof(float), &fTemp2, sizeof(float));
	}
	break;
	case OPCODE::MOD:
		iTemp1 = STACK[REGS.RSP++];
		iTemp2 = STACK[REGS.RSP++];
		iTemp2 %= iTemp1;

		STACK[--REGS.RSP] = iTemp2;
		break;
	case OPCODE::ADD:
		iTemp1 = STACK[REGS.RSP++];
		iTemp2 = STACK[REGS.RSP++];
		iTemp1 += iTemp2;

		STACK[--REGS.RSP] = iTemp1;
		break;
	case OPCODE::ADDF:
	{
		memcpy_s(&fTemp1, sizeof(float), &STACK[REGS.RSP++], sizeof(float));
		memcpy_s(&fTemp2, sizeof(float), &STACK[REGS.RSP++], sizeof(float));

		fTemp1 += fTemp2;

		memcpy_s(&STACK[--REGS.RSP], sizeof(float), &fTemp1, sizeof(float));
	}
	break;
	case OPCODE::SUB:
		iTemp1 = STACK[REGS.RSP++];
		iTemp2 = STACK[REGS.RSP++];
		iTemp2 -= iTemp1;

		STACK[--REGS.RSP] = iTemp2;
		break;
	case OPCODE::SUBF:
	{
		memcpy_s(&fTemp1, sizeof(float), &STACK[REGS.RSP++], sizeof(float));
		memcpy_s(&fTemp2, sizeof(float), &STACK[REGS.RSP++], sizeof(float));

		fTemp2 -= fTemp1;

		memcpy_s(&STACK[--REGS.RSP], sizeof(float), &fTemp2, sizeof(float));
	}
	break;
	case OPCODE::JMP_LT:
		iTemp2 = STACK[REGS.RSP++];
		iTemp1 = STACK[REGS.RSP++];

		if (iTemp1 < iTemp2)
			STACK[--REGS.RSP] = 1;
		else
			STACK[--REGS.RSP] = 0;
		break;
	case OPCODE::JMP_LTEQ:
		iTemp2 = STACK[REGS.RSP++];
		iTemp1 = STACK[REGS.RSP++];

		if (iTemp1 <= iTemp2)
			STACK[--REGS.RSP] = 1;
		else
			STACK[--REGS.RSP] = 0;
		break;
	case OPCODE::JMP_GT:
		iTemp2 = STACK[REGS.RSP++];
		iTemp1 = STACK[REGS.RSP++];

		if (iTemp1 > iTemp2)
			STACK[--REGS.RSP] = 1;
		else
			STACK[--REGS.RSP] = 0;
		break;
	case OPCODE::JMP_GTEQ:
		iTemp2 = STACK[REGS.RSP++];
		iTemp1 = STACK[REGS.RSP++];

		if (iTemp1 >= iTemp2)
			STACK[--REGS.RSP] = 1;
		else
			STACK[--REGS.RSP] = 0;
		break;
	case OPCODE::JMP_EQ:
		iTemp2 = STACK[REGS.RSP++];
		iTemp1 = STACK[REGS.RSP++];

		if (iTemp1 == iTemp2)
			STACK[--REGS.RSP] = 1;
		else
			STACK[--REGS.RSP] = 0;
		break;
	case OPCODE::JMP_NEQ:
		iTemp2 = STACK[REGS.RSP++];
		iTemp1 = STACK[REGS.RSP++];

		if (iTemp1 != iTemp2)
			STACK[--REGS.RSP] = 1;
		else
			STACK[--REGS.RSP] = 0;
		break;
	case OPCODE::LOGICALOR:
		iTemp2 = STACK[REGS.RSP++];
		iTemp1 = STACK[REGS.RSP++];

		if (iTemp1 || iTemp2)
			STACK[--REGS.RSP] = 1;
		else
			STACK[--REGS.RSP] = 0;
		break;
	case OPCODE::LOGICALAND:
		iTemp2 = STACK[REGS.RSP++];
		iTemp1 = STACK[REGS.RSP++];

		if (iTemp1 && iTemp2)
			STACK[--REGS.RSP] = 1;
		else
			STACK[--REGS.RSP] = 0;
		break;
	case OPCODE::BITWISEOR:
		iTemp2 = STACK[REGS.RSP++];
		iTemp1 = STACK[REGS.RSP++];

		STACK[--REGS.RSP] = (iTemp1 | iTemp2);
		break;
	case OPCODE::BITWISEAND:
		iTemp2 = STACK[REGS.RSP++];
		iTemp1 = STACK[REGS.RSP++];

		STACK[--REGS.RSP] = (iTemp1 & iTemp2);
		break;
	case OPCODE::BITWISEXOR:
		iTemp2 = STACK[REGS.RSP++];
		iTemp1 = STACK[REGS.RSP++];

		STACK[--REGS.RSP] = (iTemp1 ^ iTemp2);
		break;
	case OPCODE::BITWISENOT:
		iTemp1 = STACK[REGS.RSP++];

		STACK[--REGS.RSP] = (~iTemp1);
		break;
	case OPCODE::BITWISELEFTSHIFT:
		iTemp2 = STACK[REGS.RSP++];
		iTemp1 = STACK[REGS.RSP++];

		STACK[--REGS.RSP] = (iTemp1 << iTemp2);
		break;
	case OPCODE::BITWISERIGHTSHIFT:
		iTemp2 = STACK[REGS.RSP++];
		iTemp1 = STACK[REGS.RSP++];

		STACK[--REGS.RSP] = (iTemp1 >> iTemp2);
		break;
	case OPCODE::_NOT:
		iTemp1 = STACK[REGS.RSP++];
		if (iTemp1 > 0)
			STACK[--REGS.RSP] = 0;
		else
			STACK[--REGS.RSP] = 1;
		break;
	case OPCODE::LDA:
	{
		// LDA - Load Value from memory address in Accumulator(in our case, the STACK)
		lda(eOpCode);
	}
	break;
	case OPCODE::STA:
	{
		// STA - Store Value in Accumulator(in our case, the STACK) to memory address
		sta(eOpCode);
	}
	break;
	case OPCODE::CLR:
	{
		clrMem(eOpCode);
	}
	break;
	case OPCODE::NEGATE:
		iTemp1 = STACK[REGS.RSP++];
		STACK[--REGS.RSP] = -iTemp1;
		break;
	case OPCODE::JMP:
		iOperand = READ_OPERAND(eOpCode);
		REGS.EIP = iOperand;
		break;
	case OPCODE::JZ:
		iOperand = READ_OPERAND(eOpCode);
		iTemp1 = STACK[REGS.RSP++];
		if (iTemp1 == 0)
			REGS.EIP = iOperand;
		break;
	case OPCODE::JNZ:
		iOperand = READ_OPERAND(eOpCode);
		iTemp1 = STACK[REGS.RSP++];
		if (iTemp1 > 0)
			REGS.EIP = iOperand;
		break;
	case OPCODE::PRTS:
	{
		iTemp1 = STACK[REGS.RSP++];

		int32_t* pDS = (int32_t*)&RAM[DS_START_OFFSET];

		int32_t iStringOffset = *(pDS + iTemp1);
		std::cout << green << &RAM[iStringOffset];
	}
	break;
	case OPCODE::PRTC:
		iTemp1 = STACK[REGS.RSP++];
		std::cout << green << (char)iTemp1;
		break;
	case OPCODE::PRTI:
		iTemp1 = STACK[REGS.RSP++];
		std::cout << green << iTemp1;
		break;
	case OPCODE::PRTF:
		fTemp1 = STACK[REGS.RSP++];
		std::cout << green << fTemp1;
		break;
	case OPCODE::MALLOC:
	{
		iTemp1 = STACK[REGS.RSP++];

		int32_t iAddress = malloc(iTemp1);
		assert(iAddress >= 0);
		STACK[--REGS.RSP] = iAddress;
#if (VERBOSE == 1)
		std::cout << "\t\t\t\t\t\t" << yellow << "[HEAP]" << blue << " Malloc(" << iTemp1 << ") @ " << iAddress << " ------ CONSUMED: " << red << getConsumedMemory() << "/" << MAX_HEAP_SIZE << std::endl;
#endif
	}
	break;
	case OPCODE::FREE:
	{
		int32_t iVariable = READ_OPERAND(eOpCode);
		{
			int32_t iAddress = getValueIn(iVariable);
			dealloc(iAddress);
		}
	}
	break;
	case OPCODE::VTBL:
	{

	}
	break;
	case OPCODE::MEMSET:
	{
		memSet(eOpCode);
	}
	break;
	case OPCODE::MEMCPY:
	{
		memCpy(eOpCode);
	}
	break;
	case OPCODE::MEMCMP:
	{
		memCmp(eOpCode);
	}
	break;
	case OPCODE::MEMCHR:
	{
		memChr(eOpCode);
	}
	break;
	case OPCODE::HLT:
		m_bRunning = false;
	break;
	}

	assert(abs(REGS.RSP) < MAX_STACK_SIZE);
	assert(REGS.EIP < MAX_BYTECODE_SIZE);
}

int64_t VirtualMachine::readOperandFor(OPCODE eOpCode)
{
	CodeMap pMachineInstruction = opCodeMap[(int)eOpCode];
	switch (pMachineInstruction.ePRIMIIVETYPE)
	{
	case PRIMIIVETYPE::INT_8:
	{
		int8_t iByte = *((int8_t*)&RAM[CS_START_OFFSET + REGS.EIP]);
		REGS.EIP += sizeof(int8_t);

		return iByte;
	}
	break;
	case PRIMIIVETYPE::INT_16:
	{
		int16_t iShort = *((int16_t*)&RAM[CS_START_OFFSET + REGS.EIP]);
		REGS.EIP += sizeof(int16_t);

		return iShort;
	}
	break;
	case PRIMIIVETYPE::INT_32:
	{
		uint32_t iInt = *((uint32_t*)&RAM[CS_START_OFFSET + REGS.EIP]);
		REGS.EIP += sizeof(uint32_t);

		return iInt;
	}
	break;
	case PRIMIIVETYPE::INT_64:
	{
		int64_t iLong = *((int64_t*)&RAM[CS_START_OFFSET + REGS.EIP]);
		REGS.EIP += sizeof(int64_t);

		return iLong;
	}
	break;
	}
}

void VirtualMachine::pushr(OPCODE eOpCode)
{
	int32_t iOperand = READ_OPERAND(eOpCode);
	switch (iOperand)
	{
	case (int)EREGISTERS::RAX:	// Accumulator
		STACK[--REGS.RSP] = REGS.RAX;
		break;
	case (int)EREGISTERS::RCX:	// Counter
		STACK[--REGS.RSP] = REGS.RCX;
		break;
	case (int)EREGISTERS::RDX:	// Data
		STACK[--REGS.RSP] = REGS.RDX;
		break;
	case (int)EREGISTERS::RBX:	// Base
		STACK[--REGS.RSP] = REGS.RBX;
		break;
	case (int)EREGISTERS::RSP:	// Stack Pointer
		STACK[--REGS.RSP] = REGS.RSP;
		break;
	case (int)EREGISTERS::RBP:	// Stack Base Pointer
		STACK[--REGS.RSP] = REGS.RBP;
		break;
	case (int)EREGISTERS::RSI:	// Source
		STACK[--REGS.RSP] = REGS.RSI;
		break;
	case (int)EREGISTERS::RDI:	// Destination
		STACK[--REGS.RSP] = REGS.RDI;
		break;
	}
}

void VirtualMachine::popr(OPCODE eOpCode)
{
	int32_t iOperand = READ_OPERAND(eOpCode);
	switch (iOperand)
	{
	case (int)EREGISTERS::RAX:	// Accumulator
		REGS.RAX = STACK[REGS.RSP++];
		break;
	case (int)EREGISTERS::RCX:	// Counter
		REGS.RCX = STACK[REGS.RSP++];
		break;
	case (int)EREGISTERS::RDX:	// Data
		REGS.RDX = STACK[REGS.RSP++];
		break;
	case (int)EREGISTERS::RBX:	// Base
		REGS.RBX = STACK[REGS.RSP++];
		break;
	case (int)EREGISTERS::RSP:	// Stack Pointer
		REGS.RSP = STACK[REGS.RSP++];
		break;
	case (int)EREGISTERS::RBP:	// Stack Base Pointer
		REGS.RBP = STACK[REGS.RSP++];
		break;
	case (int)EREGISTERS::RSI:	// Source
		REGS.RSI = STACK[REGS.RSP++];
		break;
	case (int)EREGISTERS::RDI:	// Destination
		REGS.RDI = STACK[REGS.RSP++];
		break;
	}
}

void VirtualMachine::subreg(OPCODE eOpCode)
{
	int32_t iOperand = READ_OPERAND(eOpCode);
	int32_t iOperand2 = READ_OPERAND(eOpCode);

	switch (iOperand)
	{
	case (int)EREGISTERS::RAX:	// Accumulator
		REGS.RAX += iOperand2;
		break;
	case (int)EREGISTERS::RCX:	// Counter
		REGS.RCX += iOperand2;
		break;
	case (int)EREGISTERS::RDX:	// Data
		REGS.RDX += iOperand2;
		break;
	case (int)EREGISTERS::RBX:	// Base
		REGS.RBX += iOperand2;
		break;
	case (int)EREGISTERS::RSP:	// Stack Pointer
		REGS.RSP += iOperand2;
		break;
	case (int)EREGISTERS::RBP:	// Stack Base Pointer
		REGS.RBP += iOperand2;
		break;
	case (int)EREGISTERS::RSI:	// Source
		REGS.RSI += iOperand2;
		break;
	case (int)EREGISTERS::RDI:	// Destination
		REGS.RDI += iOperand2;
		break;
	}
}

void VirtualMachine::lda(OPCODE eOpCode)
{
	int32_t iOperand = READ_OPERAND(eOpCode);	// LDA_VM_4. Pointer Variable Type(int8_t = 0xFF, int16_6 = 0xFFFF, int32_t = 0xFFFFFFFF).
	int32_t iVarType = STACK[REGS.RSP++];		// LDA_VM_3. Variable TYPE (int8_t = 1, int16_t = 2, int32_t = 4).
	int32_t iAddress = STACK[REGS.RSP++];		// LDA_VM_2. Address
	int32_t iArrayIndex = STACK[REGS.RSP++];	// LDA_VM_1. ArrayIndex.

	int8_t* pAddress_8 = (int8_t*)&HEAP[iAddress];
	pAddress_8 += (iArrayIndex * iVarType);
	int32_t* pAddress = (int32_t*)pAddress_8;

	int8_t* iLValueAddr = (int8_t*)&STACK[--REGS.RSP];
	memcpy(iLValueAddr, pAddress, iVarType);
}

void VirtualMachine::sta(OPCODE eOpCode)
{
	int32_t iVariable = READ_OPERAND(eOpCode);			// STA_VM_4. Pointer Variable.
	int32_t iVarType = STACK[REGS.RSP++];				// STA_VM_3. Variable TYPE (int8_t = 1, int16_t = 2, int32_t = 4).
	int32_t iArrayIndex = STACK[REGS.RSP++];			// STA_VM_2. ArrayIndex.
	int32_t* iRValueAddr = (int32_t*)&STACK[REGS.RSP++];	// STA_VM_1. RValue to be stored, picked up from the STACK.

	int32_t iAddress = getValueIn(iVariable);
	{
		int8_t* pAddress_8 = (int8_t*)&HEAP[iAddress];
		pAddress_8 += (iArrayIndex*iVarType);
	
		memcpy(pAddress_8, iRValueAddr, iVarType);

		#if (VERBOSE == 1)
				std::cout << "\t\t\t\t\t\t" << yellow << "[HEAP] " << blue << iAddress << "[" << iArrayIndex << "] = " << *iRValueAddr << std::endl;
		#endif
	}
}

void VirtualMachine::clrMem(OPCODE eOpCode)
{
	// arr[5..7] = 0; ------------ - (II)

	int32_t iOperand1_Variable = READ_OPERAND(eOpCode);			// 1. "arr" position in heap
	int32_t iOperand2_ArrayIndex = READ_OPERAND(eOpCode);		// 2. '5'	==> ArrayIndex.
	int32_t iOperand3_LastPos = READ_OPERAND(eOpCode);			// 3. Count (in this case, 3 i.e for 5, 6, 7)
	int32_t iOperand4_RValue = READ_OPERAND(eOpCode);			// 4. '0'	==> RValue to be stored
	int32_t iOperand5_VarType = READ_OPERAND(eOpCode);			// 5. Cast Value of Type "arr" to perform relevant 'CAST'
	int32_t iOperand6_CastValue = READ_OPERAND(eOpCode);		// 6. Variable TYPE(int8_t = 1, int16_t = 2, int32_t = 4).

	// Clear Memory
	{
		int32_t iAddress = getValueIn(iOperand1_Variable);

		int8_t* pAddress_8 = (int8_t*)&HEAP[iAddress];
		pAddress_8 += (iOperand2_ArrayIndex * iOperand5_VarType);
		int32_t iCount = (iOperand3_LastPos - iOperand2_ArrayIndex);

		for (int32_t i = 1; i <= iCount; i++)
		{
			*pAddress_8 = (iOperand4_RValue & iOperand6_CastValue);
			pAddress_8 += iOperand5_VarType;
		}
	}
}

void VirtualMachine::call(OPCODE eOpCode)
{
	int32_t iOperand1_CallAddressType = READ_OPERAND(eOpCode);
	int32_t iJumpAddress = iOperand1_CallAddressType;

	E_FUNCTIONCALLTYPE eE_FUNCTIONCALLTYPE = (E_FUNCTIONCALLTYPE)((int32_t)iOperand1_CallAddressType >> (sizeof(int16_t) * 8));
	if (eE_FUNCTIONCALLTYPE == E_FUNCTIONCALLTYPE::VIRTUAL)		// ( VIRTUAL | POS_IN_VTABLE )
	{
		/////////////////////////////// OBJECT ALIGNMENT IN HEAP ///////////////////////////
		// RCX ==>	[-VTABLE_ADDR-][--MEMBER_VAR_0--][--MEMBER_VAR_1--][--MEMBER_VAR_2--]...[-VTABLE_ADDR_BASE1-][--MEMBER_VAR_0--][--MEMBER_VAR_1--]...
		//			|<--4 bytes-->|<----4 bytes---->|<----4 bytes---->|<----4 bytes---->|...
		//
		// VTABLE ==>	[-VIRT_FUN_ADDR_0-][-VIRT_FUN_ADDR_1-][-VIRT_FUN_ADDR_2-]...
		//				|<-----4 bytes---->|<-----4 bytes---->|<-----4 bytes---->...
		////////////////////////////////////////////////////////////////////////////////////

		int32_t iPosition = (iOperand1_CallAddressType & 0x0000FFFF);							// POS_IN_VTABLE
		int32_t iVTABLEAddress = getValueIn(((int32_t)E_VARIABLESCOPE::MEMBER << 16) | 0);		// RCX ==>	[-VTABLE_ADDR-][--MEMBER_VAR_0--][--MEMBER_VAR_1--][--MEMBER_VAR_2--]...[-VTABLE_ADDR_BASE1-][--MEMBER_VAR_0--][--MEMBER_VAR_1--]...
																								//			|<--4 bytes-->|<----4 bytes---->|<----4 bytes---->|<----4 bytes---->|...

		int32_t* pIntPtr = (int32_t*)&CODE[iVTABLEAddress + (sizeof(int32_t) * iPosition)];		// VTABLE ==>	[-VIRT_FUN_ADDR_0-][-VIRT_FUN_ADDR_1-][-VIRT_FUN_ADDR_2-]...
																								//				|<-----4 bytes---->|<-----4 bytes---->|<-----4 bytes---->...

		iJumpAddress = *pIntPtr;
	}

	REGS.RBP = REGS.RSP;					// ESP is now the new EBP.
	REGS.EIP = iJumpAddress;				// Jump to the call address.
}

void VirtualMachine::sysCall(OPCODE eOpCode)
{
	int32_t iOperand = READ_OPERAND(eOpCode);;
	int16_t iStringID = (iOperand >> sizeof(int16_t) * 8);
	int16_t iArgCount = (iOperand & 0x0000FFFF);

	int32_t* pDS = (int32_t*)&RAM[DS_START_OFFSET];
	int32_t iStringOffset = *(pDS + iStringID);

	const char* sSysFuncName = (const char*)&RAM[iStringOffset];
	executeScriptFunction(sSysFuncName, iArgCount);

	REGS.RSP += iArgCount;
}

void VirtualMachine::memSet(OPCODE eOpCode)
{
	int32_t iNum = STACK[REGS.RSP++];
	int32_t iValue = STACK[REGS.RSP++];
	int32_t iPointerAddress = STACK[REGS.RSP++];

	int8_t* pAddress_8 = (int8_t*)&HEAP[iPointerAddress];

	memset(pAddress_8, iValue, sizeof(int8_t) * iNum);
}

void VirtualMachine::memCpy(OPCODE eOpCode)
{
	int32_t iNum = STACK[REGS.RSP++];
	int32_t iSrcAddress = STACK[REGS.RSP++];
	int32_t iDstAddress = STACK[REGS.RSP++];

	int8_t* pSrcAddress_8 = (int8_t*)&HEAP[iSrcAddress];
	int8_t* pDstAddress_8 = (int8_t*)&HEAP[iDstAddress];

	memcpy(pDstAddress_8, pSrcAddress_8, sizeof(int8_t) * iNum);
}

void VirtualMachine::memCmp(OPCODE eOpCode)
{
	int32_t iNum = STACK[REGS.RSP++];
	int32_t iSrcAddress = STACK[REGS.RSP++];
	int32_t iDstAddress = STACK[REGS.RSP++];

	int8_t* pSrcAddress_8 = (int8_t*)&HEAP[iSrcAddress];
	int8_t* pDstAddress_8 = (int8_t*)&HEAP[iDstAddress];

	int32_t iRetValue = memcmp(pDstAddress_8, pSrcAddress_8, sizeof(int8_t) * iNum);
	STACK[--REGS.RSP] = iRetValue;
}

void VirtualMachine::memChr(OPCODE eOpCode)
{
	int32_t iNum = STACK[REGS.RSP++];
	int32_t iValue = STACK[REGS.RSP++];
	int32_t iPointerAddress = STACK[REGS.RSP++];

	int8_t* pAddress_8 = (int8_t*)&HEAP[iPointerAddress];

	int8_t* pPosition = (int8_t*)memchr(pAddress_8, iValue, sizeof(int8_t) * iNum);
	STACK[--REGS.RSP] = (iPointerAddress + (pPosition - pAddress_8));
}

int32_t VirtualMachine::malloc(int32_t iSize)
{
	int32_t iReturnAddress = -1;
	std::vector<HeapNode>::iterator itrUnAllocList = m_vUnAllocatedList.begin();
	for (; itrUnAllocList != m_vUnAllocatedList.end(); ++itrUnAllocList)
	{
		HeapNode& pUnAllocHeapNode = *itrUnAllocList;
		if (iSize <= pUnAllocHeapNode.m_iSize)
		{
			iReturnAddress = pUnAllocHeapNode.m_pAddress;
			m_vAllocatedList.push_back(HeapNode(pUnAllocHeapNode.m_pAddress, iSize));

			if (iSize == pUnAllocHeapNode.m_iSize)
			{
				m_vUnAllocatedList.erase(itrUnAllocList);
			}
			else
			{
				pUnAllocHeapNode.m_pAddress += iSize;
				pUnAllocHeapNode.m_iSize -= iSize;
			}

			break;
		}
	}

	assert(iReturnAddress >= 0);

	return iReturnAddress;
}

bool sortList(const HeapNode& first, const HeapNode& second)
{
	return (first.m_pAddress < second.m_pAddress);
}

void VirtualMachine::dealloc(int32_t pAddress)
{
	std::vector<HeapNode>::iterator itrAllocList = m_vAllocatedList.begin();
	bool bMemoryReclaimed = false;

	/////////////////////////////////////////////////////////////////
	// 1. Iterate through the 'Allocated' list.
	for (; itrAllocList != m_vAllocatedList.end(); ++itrAllocList)
	{
		HeapNode& pAllocHeapNode = *itrAllocList;

		/////////////////////////////////////////////////////////////////
		// 2. Check if the address to be freed matches with any HeapNode.
		if (pAddress == pAllocHeapNode.m_pAddress)
		{
#if (VERBOSE == 1)
			std::cout << "\t\t\t\t\t\t" << yellow << "[HEAP]" << blue << " Reclaiming Memory @ " << pAddress << " of Size = " << pAllocHeapNode.m_iSize << " ----- AVAILABLE: " << green << getAvailableMemory() << "/" << MAX_HEAP_SIZE << std::endl;
#endif
			/////////////////////////////////////////////////////////////////
			// 3. Merge it with any preceding HeapNode.
			bool bFoundPrecedingNode = false;
			for (HeapNode& pUnAllocHeapNode : m_vUnAllocatedList)
			{
				if (pUnAllocHeapNode.m_pAddress + pUnAllocHeapNode.m_iSize == pAddress)
				{
					bFoundPrecedingNode = true;
					pUnAllocHeapNode.m_iSize += pAllocHeapNode.m_iSize;
					break;
				}
			}

			{
				std::vector<HeapNode>::iterator itr = std::find_if(m_vUnAllocatedList.begin(),
					m_vUnAllocatedList.end(),
					[pAllocHeapNode](HeapNode& pHeapNode) {
					return (pAllocHeapNode.m_pAddress >= pHeapNode.m_pAddress
						&&
						(pAllocHeapNode.m_pAddress + pAllocHeapNode.m_iSize) <= (pHeapNode.m_pAddress + pHeapNode.m_iSize)
						);
				});
				if (itr == m_vUnAllocatedList.end())
				{
					m_vUnAllocatedList.push_back(HeapNode(pAllocHeapNode.m_pAddress, pAllocHeapNode.m_iSize));

					/////////////////////////////////////////////////////////////////
					// HeapNodes can be optimized to be merged periodically,
					// depending on various factors, one of which can be:
					//		- If 'UnAllocated List' increases to a certain number.
					if (m_vUnAllocatedList.size() > 5)
					{
						std::sort(m_vUnAllocatedList.begin(), m_vUnAllocatedList.end(), sortList);
						merge();
					}
				}

				m_vAllocatedList.erase(itrAllocList);
				bMemoryReclaimed = true;
			}

			break;
		}
	}

	assert(bMemoryReclaimed);
}

void VirtualMachine::merge()
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// If 2 Heap Blocks are contiguous, Merge the "Later" into the "Former".
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::vector<HeapNode>::iterator itrUnAllocList = m_vUnAllocatedList.begin();
	int32_t iCount = 0;
	while (iCount < m_vUnAllocatedList.size())
	{
		HeapNode& pUnAllocHeapNode = m_vUnAllocatedList.at(iCount);
		if (iCount > 0)
		{
			HeapNode& pPrevUnAllocHeapNode = m_vUnAllocatedList.at(iCount - 1);
			if (pPrevUnAllocHeapNode.m_pAddress + pPrevUnAllocHeapNode.m_iSize == pUnAllocHeapNode.m_pAddress)
			{
				pPrevUnAllocHeapNode.m_iSize += pUnAllocHeapNode.m_iSize;
				m_vUnAllocatedList.erase(itrUnAllocList + iCount);
				iCount--;
			}
		}

		iCount++;
	}
}

int32_t VirtualMachine::getValueIn(int32_t iVariable)
{
	int32_t iValue = 0;
	int16_t iVariablePos = (iVariable & 0x0000FFFF);
	E_VARIABLESCOPE eVariableType = (E_VARIABLESCOPE)((int32_t)iVariable >> (sizeof(int16_t) * 8));

	/////////////////////////////////////////////////////////////////
	// Local Var or Function Argument ==> saved on the STACK
	if (eVariableType == E_VARIABLESCOPE::ARGUMENT
		||
		eVariableType == E_VARIABLESCOPE::LOCAL
		) {
		if (eVariableType == E_VARIABLESCOPE::ARGUMENT)
			iVariablePos *= -1;

		iValue = STACK[REGS.RBP - iVariablePos];
	}
	else
		if (eVariableType == E_VARIABLESCOPE::MEMBER)
		{
			/////////////////////////////// OBJECT ALIGNMENT IN HEAP ///////////////////////////
			// RCX ==>	[-VTABLE_ADDR-][--MEMBER_VAR_0--][--MEMBER_VAR_1--][--MEMBER_VAR_2--]...[-VTABLE_ADDR_BASE1-][--MEMBER_VAR_0--][--MEMBER_VAR_1--]...
			//			|<--4 bytes-->|<----4 bytes---->|<----4 bytes---->|<----4 bytes---->|...
			//
			// VTABLE ==>	[-VIRT_FUN_ADDR_0-][-VIRT_FUN_ADDR_1-][-VIRT_FUN_ADDR_2-]...
			//				|<-----4 bytes---->|<-----4 bytes---->|<-----4 bytes---->...
			////////////////////////////////////////////////////////////////////////////////////

			int32_t iAddress = REGS.RCX + (sizeof(int32_t) * iVariablePos);
			int32_t* pIntPtr = (int32_t*)(HEAP + iAddress);
			iValue = (*pIntPtr);
		}
		else // STATIC variable saved on the HEAP
		{
			iValue = GLOBALS[iVariablePos];
		}

	return iValue;
}

void VirtualMachine::store(OPCODE eOpCode)
{
	int32_t iVariable = READ_OPERAND(eOpCode);
	int16_t iVariablePos = (iVariable & 0x0000FFFF);
	E_VARIABLESCOPE eVariableType = (E_VARIABLESCOPE)((int32_t)iVariable >> (sizeof(int16_t) * 8));

	/////////////////////////////////////////////////////////////////
	// Local Var or Function Argument ==> saved on the STACK
	if (eVariableType == E_VARIABLESCOPE::ARGUMENT
		||
		eVariableType == E_VARIABLESCOPE::LOCAL
		) {
		if (eVariableType == E_VARIABLESCOPE::ARGUMENT)
			iVariablePos *= -1;

		STACK[REGS.RBP - iVariablePos] = STACK[REGS.RSP++];
	}
	else
		if (eVariableType == E_VARIABLESCOPE::MEMBER)
		{
			int32_t* iIntPtr = (int32_t*)(HEAP + REGS.RCX);
			iIntPtr += iVariablePos;
			*iIntPtr = STACK[REGS.RSP++];
		}
		else // STATIC variable saved on the HEAP
		{
			GLOBALS[iVariablePos] = STACK[REGS.RSP++];
		}
}

int32_t VirtualMachine::getConsumedMemory()
{
	int32_t iConsumedMemory = 0;
	for (HeapNode pHeapNode : m_vAllocatedList)
	{
		iConsumedMemory += pHeapNode.m_iSize;
	}

	return iConsumedMemory;
}

int32_t VirtualMachine::getAvailableMemory()
{
	int32_t iAvailableMemory = 0;
	for (HeapNode pHeapNode : m_vUnAllocatedList)
	{
		iAvailableMemory += pHeapNode.m_iSize;
	}

	return iAvailableMemory;
}

void* VirtualMachine::getStackPointerFromTOS(int32_t iOffset)
{
	assert(abs(REGS.RSP + iOffset) < MAX_STACK_SIZE);
	return &STACK[REGS.RSP + iOffset];
}

// Dummy System Functions.
void glLoadIdentity()
{
	std::cout << blue << "In glLoadIdentity();" << std::endl;
}

void glClearColor(int32_t iRed, int32_t iGreen, int32_t iBlue, int32_t iAlpha)
{
	std::cout << blue << "In glClearColor(" << iRed << ", " << iGreen << ", " << iBlue << ", " << iAlpha << ");" << std::endl;
}

void glColor3f(float fRed, float fGreen, float fBlue)
{
	std::cout << blue << "In glColor3f(" << fRed << ", " << fGreen << ", " << fBlue << ");" << std::endl;
}

int32_t retFunc(int32_t iValue)
{
	std::cout << blue << "In iValue(" << iValue << ", retValue = " << (iValue * 3) << ");" << std::endl;
	return iValue * 3;
}

META_REGISTER_FUN(glLoadIdentity);
META_REGISTER_FUN(glClearColor);
META_REGISTER_FUN(glColor3f);
META_REGISTER_FUN(retFunc);

void VirtualMachine::scriptTest()
{
	// Script Test
	{
		GetFunctionByName("glLoadIdentity")->call(nullptr, nullptr, 0);

		int32_t iRed = 128, iGreen = 64, iBlue = 32, iAlpha = 255;
		Variable iArgs[] = { iRed, iGreen, iBlue, iAlpha };
		GetFunctionByName("glClearColor")->call(nullptr, iArgs, 4);

		float fRed = 128.1f, fGreen = 64.2f, fBlue = 32.3f;
		Variable fArgs[] = { fRed, fGreen, fBlue };
		GetFunctionByName("glColor3f")->call(nullptr, fArgs, 4);

		int32_t iRetValue = 0;
		Variable iRetVar(iRetValue);
		Variable iArg(iRed);
		GetFunctionByName("retFunc")->call(iRetVar, &iArg, 1);
	}
}

void VirtualMachine::executeScriptFunction(const char* sFuncName, int32_t iArgCount)
{
	MetaFunction* pMetaFunction = GetFunctionByName(sFuncName);
	assert(pMetaFunction != nullptr);
	if (pMetaFunction != nullptr)
	{
		// Return Value
		Variable ret;
		{
			ret.m_MetaType = pMetaFunction->getRetType();
			if (ret.m_MetaType->sizeOf() > 0)
				ret.m_Var = ret.m_MetaType->newAlloc();
		}

		// Function Arguments
		Variable* pArgs = new Variable[iArgCount];// (Variable*)::malloc(iArgCount * sizeof(Variable));
		{
			for (int32_t i = 0; i < pMetaFunction->getArgCount(); i++)
			{
				const MetaType* pArgMetaType = pMetaFunction->getArgType(i);
				pArgs[i].m_MetaType = pArgMetaType;
				pArgs[i].m_Var = pArgs[i].m_MetaType->newAlloc();
				pArgs[i].m_MetaType->setValueFrom(pArgs[i].m_Var, getStackPointerFromTOS(i));
			}
		}

		// Function Call
		pMetaFunction->call(ret, pArgs, iArgCount);

		// Check Return type & push it on the 'STACK'.
		size_t iRetVarSize = ret.m_MetaType->sizeOf();
		if (iRetVarSize > 0)
		{
			memcpy_s(&STACK[--REGS.RSP], iRetVarSize, ret.m_Var, iRetVarSize);
		}

		// Function Cleanup
		{
			for (int32_t i = 0; i < pMetaFunction->getArgCount(); i++)
			{
				pArgs[i].m_MetaType->deleteAlloc(pArgs[i].m_Var);
			}
			delete[] pArgs;
			if (ret.m_MetaType->sizeOf() > 0)
				ret.m_MetaType->deleteAlloc(ret.m_Var);
		}
	}
}
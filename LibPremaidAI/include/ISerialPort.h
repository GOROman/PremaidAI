#pragma once
#include "Common.h"

#define ISerialPort_API_VER (100)


// Serial Port Interface
class  ISerialPort {
public:
	enum PARITY {
		NONE = 0,
		ODD = 1,
		EVEN = 2,
	};

public:
	virtual ~ISerialPort() {}

	// �V���A���|�[�g�֐ڑ�
	virtual RESULT Connect(const wchar_t* portname, const int baudrate, const PARITY parity) = 0;

	// �V���A���|�[�g����ؒf
	virtual RESULT Disconnect() = 0;

	// �V���A���|�[�g����ǂݍ���
	virtual RESULT Read(unsigned char* buffer, const size_t size, size_t* read_size) = 0;

	// �V���A���|�[�g�֏�������
	virtual RESULT Write(const unsigned char* buffer, const size_t size, size_t* written_size) = 0;

	// �w�莞��(ms)�ҋ@
	virtual RESULT Sleep(const int ms) = 0;
};

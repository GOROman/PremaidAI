#pragma once

#include "Common.h"
#include "ISerialPort.h"

#define IServo_API_VER (100)

// Servo Motor Interface
class IServo {
public:
	virtual ~IServo() {}

	// �V���A���|�[�g���A�^�b�`
	virtual RESULT Attach(ISerialPort* serial) = 0;

	// ���_���Z�b�g
	virtual RESULT Reset(const int id) = 0;

	// �E�͂�����
	virtual RESULT SetFree(const int id) = 0;

	// �T�[�{���w��ʒu�֓�����
	virtual RESULT SetPosition(const int id, const int pos) = 0;

	// �T�[�{�̌��݈ʒu���擾
	virtual RESULT GetPosition(const int id, int* pos) = 0;

	// �p�����[�^���Z�b�g����
	virtual RESULT SetParam(const int id, const int PARAMTYPE, const int param) = 0;

	// �p�����[�^���擾����
	virtual RESULT GetParam(const int id, const int PARAMTYPE, int* param) = 0;

	// �T�[�{���x�擾
	virtual RESULT GetTemp(const int id, float* temp) = 0;

	// �T�[�{�d���擾
	virtual RESULT GetCurrent(const int id, float* current) = 0;

	// �A�b�v�f�[�g
	virtual RESULT Update() = 0;
};

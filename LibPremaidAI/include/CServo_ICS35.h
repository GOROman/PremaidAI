#pragma once

//
// �ߓ��Ȋw ICS 3.5 ���� (https://kondo-robot.com/faq/ics3-5-explain)
//

#include "Common.h"
#include "IServo.h"
#include <deque>

#define ICS_ID_MIN		(0)		// �T�[�{ID �ŏ��l
#define ICS_ID_MAX		(31)	// �T�[�{ID	�ő�l
#define ICS_ID_COUNT	(32)	// �ő�T�[�{��

#define ICS_POS_CENTER	(7500)	// �T�[�{�����ʒu
#define ICS_POS_MIN		(3500)	// �T�[�{�ŏ��ʒu
#define ICS_POS_MAX		(11500)	// �T�[�{�ő�ʒu

#define ICS_STEP		(135.0f/4000.0f)	// �T�[�{1�X�e�b�v������̊p�x

#define DEG2POS(deg)	((deg)/ICS_STEP + ICS_POS_CENTER)
#define POS2DEG(pos)	(((pos)-ICS_POS_CENTER)*ICS_STEP)


typedef unsigned char DATABYTE;


class CServo_ICS35 : public IServo {
public:
	enum {
		PARAMTYPE_STRETCH = 1,		// �X�g���b�`�f�[�^
		PARAMTYPE_SPEED = 2,		// �X�s�[�h�f�[�^
		PARAMTYPE_CURRENT = 3,		// �d���l
		PARAMTYPE_TEMPERATURE = 4,	// ���x�l
	};

public:
	// �R���X�g���N�^
	CServo_ICS35();

	// �f�X�g���N�^
	virtual ~CServo_ICS35();

	// �V���A���|�[�g���A�^�b�`
	virtual RESULT Attach(ISerialPort* serial);

	// ���_���Z�b�g
	virtual RESULT Reset(const int id);

	// �E�͂�����
	virtual RESULT SetFree(const int id);

	// �T�[�{���w��ʒu�֓�����
	virtual RESULT SetPosition(const int id, const int pos);

	// �T�[�{�̌��݈ʒu���擾
	virtual RESULT GetPosition(const int id, int* pos);

	// �p�����[�^���Z�b�g����
	virtual RESULT SetParam(const int id, const int PARAMTYPE, const int param);

	// �p�����[�^���擾����
	virtual RESULT GetParam(const int id, const int PARAMTYPE, int* param);

	// �T�[�{���x�擾
	virtual RESULT GetTemp(const int id, float* temp);

	// �T�[�{�d���擾
	virtual RESULT GetCurrent(const int id, float* current);

	// �A�b�v�f�[�g
	virtual RESULT Update();

protected:

	// ��M�f�[�^���n���h�����O
	size_t Proc(size_t count, int* data);

	// �T�[�{�����M
	RESULT RecvData(size_t count);

	// �T�[�{�փR�}���h���M���A���ʂ���M
	RESULT SendCommand(const DATABYTE byte1, const DATABYTE byte2, const DATABYTE byte3);

	// �T�[�{�փR�}���h���M���A���ʂ���M
	RESULT ReadCommand(const DATABYTE byte1, const DATABYTE byte2, int* data);

private:
	ISerialPort* _serial;

	int _pos[ICS_ID_COUNT];

	std::deque<DATABYTE>	_buffer;
};


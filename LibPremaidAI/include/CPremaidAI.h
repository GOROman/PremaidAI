#pragma once

#ifdef LIBPREMAIDAI_DLL_EXPORT
#else
#define LIBPREMAIDAI_DLL_EXPORT  __declspec(dllimport)
#endif

#include <Common.h>

#include <ISerialPort.h>
#include <IServo.h>

enum  {
	SHOULDER_PITCH_R = 1,	// ���s�b�`R
	SHOULDER_PITCH_L = 3,	// ���s�b�`L
};
/*
1	���s�b�`R
1	���s�b�`
2	���s�b�`L
2	�����[
3	�q�b�v���[R
3	���G
4	�q�b�v���[L
4	�����[��R
5	�q�b�v���[��R
5	�����[��L
6	�q�b�v���[��L
6	��r���[R
7	�ڃs�b�`R
7	��r���[L
8	�ڃs�b�`L
8	�I�s�b�`R
9	�G�s�b�`R
9	�I�s�b�`L
10	�G�s�b�`L
10	��񃈁[R
11	����s�b�`R
11	��񃈁[L
12	����s�b�`L
13	���񃍁[��R
14	���񃍁[��L
*/

#define ICS_COUNT		(2)

enum {
	TYPE_HV = 0,
	TYPE_MV = 1,
};

#define MV_SERVO_ID_COUNT  (11)	// MV�T�[�{ID��
#define HV_SERVO_ID_COUNT  (14)	// HV�T�[�{ID��

#define SERVO_COUNT     (MV_SERVO_ID_COUNT + HV_SERVO_ID_COUNT)

class LIBPREMAIDAI_DLL_EXPORT CPremaidAI {
	bool	_isConnected;

	ISerialPort* _serial;
	IServo* _ics[ICS_COUNT];

public:
	CPremaidAI();

	virtual ~CPremaidAI();

	ISerialPort* GetSerialPort() { return _serial;  }

//	IServo* GetServo(const int type) {
//	}

	// �ڑ�����
	virtual void Connect(const wchar_t* port, const int baudrate);

	// �ؒf����
	virtual void Disconnect();

	// �T�[�{�ʒu��ݒ�
	virtual void SetPosition(const int id, const int pos);

	// �p�x(�x)���w�肵�ăT�[�{�ʒu��ݒ�
	virtual void SetRotation(const int id, const float deg);

	// ���ׂẴT�[�{�ʒu���ꊇ�w��
	virtual void SetPositionAll(const int pos);

	// �T�[�{�ʒu�擾
	virtual int GetPosition(const int id) ;

	// �X�g���b�`�ݒ�
	virtual void SetStrectch(const int id, const int stretch) ;

	// �T�[�{�X�s�[�h�ݒ�
	virtual void SetSpeed(const int id, const int speed) ;

	// �T�[�{���x�𓾂�
	virtual float GetTemperature(const int id);

	// �T�[�{�d���l�𓾂�
	virtual float GetCurrent(const int id);

	// �X�V
	virtual void Update();
		

};

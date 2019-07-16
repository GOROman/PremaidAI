
//#include <windows.h>


#include "Common.h"
#include "CServo_ICS35.h"

// �R�}���h
#define ICS_CMD_POS		(0x80)	// 100_ ____
#define ICS_CMD_READ	(0xA0)	// 101_ ____
#define ICS_CMD_WRITE	(0xC0)	// 110_ ____
#define ICS_CMD_ID		(0xE0)	// 111_ ____

#define ICS_CMD_MASK	(0xE0)	// 111_ ____


// �T�u�R�}���h
#define ICS_SC_EEPROM	(0x00)	// �T�[�{���[�^��ROM�f�[�^�ɒ��ڃA�N�Z�X����
#define ICS_SC_STRC		(0x01)	// �X�g���b�`�f�[�^
#define ICS_SC_SPD		(0x02)	// �X�s�[�h�f�[�^
#define ICS_SC_CUR		(0x03)	// �d���l
#define ICS_SC_TMP		(0x04)	// ���x�l

#define BUFFER_SIZE		(1024)

#ifdef _DEBUG
#define ICS_DEBUG		(1)		// �f�o�b�O���[�h
#else
#define ICS_DEBUG		(0)
#endif

#if ICS_DEBUG
#define ICS_TRACE	printf
#else
#define ICS_TRACE	__noop
#endif

// �R���X�g���N�^
CServo_ICS35::CServo_ICS35() {
	_pos;
	_serial = NULL;
	for (int i = 0; i < ICS_ID_COUNT; ++i) {
		_pos[i] = ICS_POS_CENTER;
	}
}

// �f�X�g���N�^
CServo_ICS35::~CServo_ICS35() {
	_buffer.clear();
}

// �V���A���|�[�g���A�^�b�`
RESULT CServo_ICS35::Attach(ISerialPort* serial) {
	assert(_serial == NULL);
	_serial = serial;
	return RESULT_OK;
}

// ���_���Z�b�g
RESULT CServo_ICS35::Reset(const int id) {
	return SetPosition(id, ICS_POS_CENTER);
}

// �E�͂�����
RESULT CServo_ICS35::SetFree(const int id) {
	return SetPosition(id, 0);
}

// �T�[�{���w��ʒu�֓�����
RESULT CServo_ICS35::SetPosition(const int id, const int pos) {
	int new_pos = pos;
	// �N�����v
	if (new_pos == 0) {
		// �E�͎w��
	}
	else if (new_pos > ICS_POS_MAX) {
		new_pos = ICS_POS_MAX;
	}
	else if (new_pos < ICS_POS_MIN) {
		new_pos = ICS_POS_MIN;
	}

	SendCommand(ICS_CMD_POS + id, new_pos >> 7, new_pos & 0x7f);

	return RESULT_OK;
}

// �T�[�{�̌��݈ʒu���擾
RESULT CServo_ICS35::GetPosition(const int id, int* pos) {
	assert(id >= ICS_ID_MIN && id <= ICS_ID_MAX);
	if (id >= ICS_ID_MIN && id <= ICS_ID_MAX) {
		*pos = _pos[id];
		return RESULT_OK;
	}
	return RESULT_ERROR;
}

// �p�����[�^��ICS�̃T�u�R�}���h�o�C�g�ɕϊ�
static DATABYTE ConvertParamtypeToSubCommand(const int PARAMTYPE)
{
	DATABYTE sc = 0x00;
	switch (PARAMTYPE) {
	case CServo_ICS35::PARAMTYPE_SPEED:			sc = ICS_SC_SPD; break;
	case CServo_ICS35::PARAMTYPE_STRETCH:		sc = ICS_SC_STRC; break;
	case CServo_ICS35::PARAMTYPE_CURRENT:		sc = ICS_SC_CUR; break;
	case CServo_ICS35::PARAMTYPE_TEMPERATURE:	sc = ICS_SC_TMP; break;
	default:
		assert(0);
	}
	return sc;
}


// �p�����[�^���Z�b�g����
RESULT CServo_ICS35::SetParam(const int id, const int PARAMTYPE, const int param) {
	assert(id >= ICS_ID_MIN && id <= ICS_ID_MAX);

	SendCommand(ICS_CMD_WRITE + id, ConvertParamtypeToSubCommand(PARAMTYPE), param);
	return RESULT_OK;
}

// �p�����[�^���擾����
RESULT CServo_ICS35::CServo_ICS35::GetParam(const int id, const int PARAMTYPE, int* param) {
	assert(id >= ICS_ID_MIN && id <= ICS_ID_MAX);

	ReadCommand(ICS_CMD_READ + id, ConvertParamtypeToSubCommand(PARAMTYPE), param);
	return RESULT_OK;
}

// �T�[�{���x�擾
RESULT CServo_ICS35::GetTemp(const int id, float* temp) {
	assert(id >= ICS_ID_MIN && id <= ICS_ID_MAX);
	int param = 0;
	GetParam(id, PARAMTYPE_TEMPERATURE, &param);

#if 0
	// �d�l���ʂ�
	if (param < 30) {
		*temp = 100.0f;
	}
	else if (param < 47) {
		*temp = 90.0f;
	}
	else if (param < 60) {
		*temp = 80.0f;
	}
	else if (param < 75) {
		*temp = 70.0f;
	}
	else if (param < 87) {
		*temp = 60.0f;
	}
	else {
		*temp = 50.0f;	//��
	}
#else
	//�ŏ����@�ŋߎ�
	if (param == 0) {
		// �G���[?
		(*temp) = -1.0f;
	}
	else {
		(*temp) = -0.7019972316f * param + 121.9794344f;
	}
#endif
	return RESULT_OK;
}

// �T�[�{�d���擾
RESULT CServo_ICS35::GetCurrent(const int id, float* current) {
	assert(id >= ICS_ID_MIN && id <= ICS_ID_MAX);
	int param = 0;
	GetParam(id, PARAMTYPE_CURRENT, &param);

	float  a = (float)param;
	if (a >= 64.0f) {
		a = (a - 64.0f) * -1.0f;	// �t�]
	}
	*current = (float)a * 0.1f;

	return RESULT_OK;
}

// �A�b�v�f�[�g
RESULT CServo_ICS35::Update() {
	return RESULT_OK;
}


#define GETDATA _buffer.front(); _buffer.pop_front(); 
static DATABYTE cmd = 0x00;

// ��M�f�[�^���n���h�����O
size_t CServo_ICS35::Proc(size_t count, int* data)
{
#if 1
	// �R�}���h�̃p�[�X���s��
	if (_buffer.size() >= count) {
		ICS_TRACE("<%zd>", _buffer.size());

		cmd = GETDATA;	// �G�R�[�o�b�N
		if (cmd == 0xff) {
			printf("PARITY ERROR!!\n");
			return _buffer.size();
		}
		if ((cmd & 0x80) == 0x00) {	// MSB�͗����Ă��邩�H
			// �����ĂȂ��Ȃ�X�L�b�v����
			ICS_TRACE("=== ��肱�ڂ��I[%02x]\n", cmd);
			return _buffer.size();
		}

		DATABYTE p1, p2, p3, p4, r_cmd;
		switch (cmd & ICS_CMD_MASK) {
		case ICS_CMD_POS:
		{
			// �G�R�[�o�b�N
			p1 = GETDATA; p2 = GETDATA;	r_cmd = GETDATA; p3 = GETDATA; p4 = GETDATA;

			int id = r_cmd & 0x1f;
			int pos = (p3 << 7) + p4;
			if ((id >= ICS_ID_MIN) && (id <= ICS_ID_MAX)) {
				_pos[id] = pos;
			}
			ICS_TRACE("[%02x %02x %02x] ", cmd, p1, p2);
			ICS_TRACE("[%02x %02x %02x] POS ID:%2d POS:%d", r_cmd, p3, p4, id, pos);
		}
		break;

		case ICS_CMD_WRITE:
		{
			p1 = GETDATA; p2 = GETDATA;	r_cmd = GETDATA; p3 = GETDATA; p4 = GETDATA;
			ICS_TRACE("[%02x %02x %02x] ", cmd, p1, p2);
			ICS_TRACE("[%02x %02x %02x] SC_WRITE: ID:%2d %d=%d", r_cmd, p3, p4, r_cmd & 0x1f, p3, p4);
		}
		break;

		case ICS_CMD_READ:
		{
			p1 = GETDATA; r_cmd = GETDATA; p3 = GETDATA; p4 = GETDATA;
			*data = p4;
			ICS_TRACE("[%02x %02x] ", cmd, p1);
			ICS_TRACE("[%02x %02x %02x] SC_READ: ID:%2d [%02x] %d %d", r_cmd, p3, p4, r_cmd & 0x1f, p1, p3, p4);
		}
		break;
		default:
			printf("Error: Unknown Data[%02x]\n", cmd);
			assert(0);
		}
	}

	ICS_TRACE("\n");

	return _buffer.size();
#endif
}
// �T�[�{�����M
RESULT CServo_ICS35::RecvData(size_t count )
{
	if (_serial == NULL) return RESULT_ERROR;

	int timeout = 100;	// 10ms
	DATABYTE buffer[BUFFER_SIZE];

	while (1) {
		size_t len = 0;
		// �V���A�������M
		_serial->Read(buffer, count, &len);
		if (len > 0) {
			// �o�b�t�@�����O
			for (size_t i = 0; i < len; ++i) {
				if (buffer[i] == 0xff) {	// �p���e�B�G���[
					printf("��");
				}
				else {
					_buffer.push_back(buffer[i]);
				}
			}
			break;
		} else {
			ICS_TRACE(".");
			_serial->Sleep(1);
			timeout--;
			if (timeout <= 0) {
				ICS_TRACE("Timeout!\n");
				break;
			}
		}
	}

	return RESULT_OK;
}

// �T�[�{�փR�}���h���M���A���ʂ���M
RESULT CServo_ICS35::SendCommand(const DATABYTE byte1, const DATABYTE byte2, const DATABYTE byte3)
{
	if (_serial == NULL) return RESULT_ERROR;

	size_t sendsize = 0;

	DATABYTE buffer[] = { byte1, byte2, byte3 };
	RESULT result = _serial->Write(buffer, sizeof(buffer), &sendsize);
	ICS_TRACE("SendCommand(%02x,%02x,%02x) sendsize:%zd\n", byte1, byte2, byte3, sendsize);

	assert(sizeof(buffer) == sendsize);
#if 0
	result = RecvData(6);

	int data = 0;
	while (Proc(3 + 3, &data) >= 6) {
		printf("*");
	}// �o�b�t�@����ɂȂ�܂ŏ���
#endif

	return result;
}

// �T�[�{�փ��[�h�R�}���h���M���A���ʂ���M
RESULT CServo_ICS35::ReadCommand(const DATABYTE byte1, const DATABYTE byte2, int* data)
{
	if (_serial == NULL) return RESULT_ERROR;

	size_t sendsize = 0;

	DATABYTE buffer[] = { byte1, byte2 };
	RESULT result = _serial->Write(buffer, sizeof(buffer), &sendsize);
	ICS_TRACE("ReadCommand(%02x,%02x) sendsize:%zd\n", byte1, byte2, sendsize);

	assert(sizeof(buffer) == sendsize);

	result = RecvData(5);
	Proc(2 + 3, data);

	return result;
}


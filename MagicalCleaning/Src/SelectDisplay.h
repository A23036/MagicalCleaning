#pragma once
#include "Object3D.h"
#include "CsvSelectDataLoader.h"

//�v���g�^�C�v�錾
class DataCarrier;
class EasingCalc;

// ---------------------------------------------------------------------------
// �Z���N�g���UI�`�揈��
// ---------------------------------------------------------------------------
class SelectDisplay : public Object3D
{
public:
	SelectDisplay();
	virtual ~SelectDisplay();

	void Init();
	void Update() override;
	void Draw() override;

	void DrawUI();		//�v���C���[�̏�Ԃɉ�����UI�̕`��
	void Transition();	//��ʐ؂�ւ�����

	//�I����Ԃɉ������X�v���C�g�`�揈��
	void UpdateColorSelect(int playerNum,int ix,int iy);
	void UpdateCameraSetting(int playerNum,int ix,int iy);
	void UpdateReady(int playerNum);
	void UpdateEnd(int playerNum);

	void CsvLoad(); //CSV�t�@�C������ǂݍ��񂾃f�[�^�̐ݒ���s��

	//�A�N�Z�X�֐�
	bool GetIsSelectColor(int playerNum) { return playerEntry[playerNum]; };
	bool GetIsSelected(int color) { return isSelect[color]; };
	int  GetSelectColor(int playerNum) { return selectColor[playerNum]; };
	bool GetIsReady() { return isReadyAll; };
	
private:
	//�e��N���X�̃|�C���^
	DataCarrier* dc;
	EasingCalc* ec;
	CSprite* sprite;
	CSpriteImage* selectUiImage;	//�Z���N�g���UI�C���[�W
	
	//�L�����N�^�[�J���[
	enum Color {
		Red = 0,
		Blue,
		Yellow,
		Green,
		Purple,
		Black,
	};

	//�ݒ���
	enum SettingState {
		sColor,
		sCamera,
		sReady,
		sEnd,
	};

	SettingState state[MAXPLAYER]; //�v���C���[���Ƃ̐ݒ��ԕۑ��p�z��
	
	//CSV�t�@�C������ǂݍ��񂾃f�[�^���i�[����\����
	SelectDisParams selectDisParams;

	//CSV�t�@�C������ǂݍ��܂��f�[�^
	int ColorPosY;			//�L�����I���J���[�ʒuY
	int ColorIconSize;		//�L�����I���J���[�T�C�Y
	int ColorIconDispSize;	//�L�����I���J���[�\���T�C�Y
	int UiSpace;			//UI���m�̊Ԋu
	int GuideUiPosY;		//������@�K�C�hUI�ʒuY
	int MoveFrm;			//�J�[�\���������̂ɕK�v�ȓ��̓t���[����
	VECTOR2 BackUiPos;		//�߂�UI�ʒu


	bool isReadyAll;				//�v���C���[�S������������
	bool playerEntry[MAXPLAYER];	//�v���C���[�̃G���g���[���
	bool isReady[MAXPLAYER];		//�v���C���[������������
	bool isSetUpCamera[MAXPLAYER];	//�v���C���[���Ƃ̃J�����ݒ芮������
	bool isSelect[6];				//�J���[���Ƃ̑I���ςݔ���
	bool isTransition;				//�g�����W�V����������
	bool isFirst;					//����g�����W�V��������
	int  selectColor[MAXPLAYER];	//�v���C���[���Ƃ̑I�𒆃J���[
	int  selectCamera[MAXPLAYER];	//�v���C���[���Ƃ̑I�𒆃J�����ݒ� 0:�I�[�g/1:�}�j���A��
	int moveFrm[MAXPLAYER];			//�v���C���[���Ƃ̓��̓t���[���J�E���g�p�ϐ�
	int animFrm[MAXPLAYER];			//�v���C���[���Ƃ̏��������A�j���[�V�����o�߃t���[���ۑ�
	int  transFrm;					//�g�����W�V�����t���[��
	VECTOR2 cursorPos[MAXPLAYER];	//�J�[�\���ʒu
};
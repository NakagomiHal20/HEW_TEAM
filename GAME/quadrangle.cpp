//=============================================================================
// �l�p�`���� [quadrangle.cpp]
//
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP11B341 24 �����a�P
// �쐬�� : 2018/12/26
//=============================================================================


//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "quadrangle.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************


//=============================================================================
// �l�p�`�Q�c�|���S���̒��_�����쐬
//=============================================================================
HRESULT MakeVertexQuad(QUADRANGLE *quad)
{
	// ���_���W�̐ݒ�
	SetVertexQuad(quad);

	// rhw�̐ݒ�
	quad->vertexWk[0].rhw =
	quad->vertexWk[1].rhw =
	quad->vertexWk[2].rhw =
	quad->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	D3DCOLOR color = D3DCOLOR_RGBA(255, 255, 255, 255);
	SetColorQuad(quad, color);

	// �e�N�X�`�����W�̐ݒ�
	quad->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	quad->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	quad->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	quad->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}


//=============================================================================
// �l�p�`�Q�c�|���S���̒��_���W�̐ݒ�
//=============================================================================
void SetVertexQuad(QUADRANGLE *quad)
{
	// �A�t�B���ϊ�
	quad->vertexWk[0].vtx.x = quad->transform.pos.x - cosf(quad->centralAngle + quad->transform.rot.z) * quad->radius;
	quad->vertexWk[0].vtx.y = quad->transform.pos.y - sinf(quad->centralAngle + quad->transform.rot.z) * quad->radius;
	quad->vertexWk[1].vtx.x = quad->transform.pos.x + cosf(quad->centralAngle - quad->transform.rot.z) * quad->radius;
	quad->vertexWk[1].vtx.y = quad->transform.pos.y - sinf(quad->centralAngle - quad->transform.rot.z) * quad->radius;
	quad->vertexWk[2].vtx.x = quad->transform.pos.x - cosf(quad->centralAngle - quad->transform.rot.z) * quad->radius;
	quad->vertexWk[2].vtx.y = quad->transform.pos.y + sinf(quad->centralAngle - quad->transform.rot.z) * quad->radius;
	quad->vertexWk[3].vtx.x = quad->transform.pos.x + cosf(quad->centralAngle + quad->transform.rot.z) * quad->radius;
	quad->vertexWk[3].vtx.y = quad->transform.pos.y + sinf(quad->centralAngle + quad->transform.rot.z) * quad->radius;
}


//=============================================================================
// �l�p�`�Q�c�|���S�����_�J���[�̐ݒ�
//=============================================================================
void SetColorQuad(QUADRANGLE *quad, D3DCOLOR setColor)
{
	// �ꊇ�ݒ�
	quad->vertexWk[0].diffuse = setColor;
	quad->vertexWk[1].diffuse = setColor;
	quad->vertexWk[2].diffuse = setColor;
	quad->vertexWk[3].diffuse = setColor;
}


//=============================================================================
// �l�p�`�Q�c�|���S���̏�����
//=============================================================================
void InitQuad(QUADRANGLE *quad, int sizeX, int sizeY)
{
	quad->centralAngle = atan2f((float)sizeY / 2, (float)sizeX / 2);			// ���S���W���璸�_�ւ̊p�x��������
	D3DXVECTOR2 vec = D3DXVECTOR2((float)sizeX / 2, (float)sizeY / 2);			// ���S���W���璸�_�ւ̃x�N�g�������߂�
	quad->originalRadius = quad->radius = D3DXVec2Length(&vec);					// ���a�̏�����
	quad->transform.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// ���W�̏�����
	quad->transform.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// ��]�̏�����
	MakeVertexQuad(quad);														// �l�p�`�Q�c�|���S���̒��_�����쐬
}





/* �O���[�o�� */
Texture2D		g_texColor:register(t0);	//�e�N�X�`���[�̓��W�X�^�[t(n)
SamplerState	g_samLinear:register(s0);	//�T���v���[�̓��W�X�^�[s(n)

cbuffer global
{
	matrix g_mWVP;		//���[���h����ˉe�܂ł̕ϊ��s��
	float4 g_vDiffuse;	//�f�B�t���[�Y�F
	float4 g_fAlpha;	//����
	float4 g_TexScroll;	//�e�N�X�`�����[��
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
};

cbuffer Rectangle
{
	float Rect_w;
	float Rect_h;
	float Rect_l;
	float Rect_t;
};


/* �o�[�e�b�N�X�V�F�[�_�[ */
VS_OUTPUT VS(float4 Pos : POSITION, float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, g_mWVP);
	output.Tex = Tex;
	output.Tex.x += g_TexScroll.x;
	return output;
}

/* �s�N�Z���V�F�[�_�[ */
float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 color = (g_vDiffuse*g_texColor.Sample(g_samLinear, input.Tex))*g_fAlpha.x;
	clip(color.w - 0.5);
	return color;
}
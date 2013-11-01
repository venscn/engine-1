#include "Button.h"

enum ButtonInput
{
	LeftButtonPressed,
	LeftButtonReleased,
};

namespace star
{
	Button::Button(tstring assetFile, bool isSpriteSheet = false)
		: Object()
		, m_OnClick(nullptr)
		, m_OnHover(nullptr)
		, m_OnLeave(nullptr)
		, m_IsSpriteSheet(isSpriteSheet)
	{
		
	}


	Button::~Button(void)
	{
	}

	void Button::Initialize()
	{
		/*
		SpriteComponent::InitializeComponent();

		m_HitRegion = new GameHitRegion2D();
		m_pParentGameObject->AddChild(m_HitRegion);
		m_HitRegion->CanDraw(true);

		ID3D10Resource *resource;
		ID3D10Texture2D *tex2D;
		D3D10_TEXTURE2D_DESC tex2DDesc;
		m_SpriteInfo.pTexture->GetResource(&resource);
		tex2D = static_cast<ID3D10Texture2D*>(resource);
		tex2D->GetDesc(&tex2DDesc);

		m_Width = (int)tex2DDesc.Width;
		//Set Dimensions after the button texture is loaded, otherwise we can't extract the width and height
		if(m_IsSpriteSheet)
		{
			m_Height = (int)tex2DDesc.Height/3;

			//Only use these rectangle properties when the button texture is a spriteSheet
			m_NormalRect =	GameRectangle(	0,		0,									(int)tex2DDesc.Width,	(int)(tex2DDesc.Height / 3.0f));
			m_HoverRect =	GameRectangle(	0,		(int)(tex2DDesc.Height / 3.0f),		(int)tex2DDesc.Width,	2*(int)(tex2DDesc.Height / 3.0f));
			m_PressedRect = GameRectangle(	0,		2*(int)(tex2DDesc.Height / 3.0f),	(int)tex2DDesc.Width,	(int)(tex2DDesc.Height));

			m_SpriteInfo.DrawRect = GameRectangle::AsRect(m_NormalRect);
		}
		else
			m_Height = (int)tex2DDesc.Height;

		m_HitRegion->SetDimensions(m_Width, m_Height);*/

	}

	void Button::Update(const Context& context)
	{/*
		SpriteComponent::Update(context);
		m_HitRegion->SetDimensions(m_Width, m_Height);
	*/

	
	}

	void Button::Draw(const Context& context)
	{/*
		SpriteComponent::Draw(context);

		POINT currMousePos = context.Input->GetMousePosition();
		if(!m_IsHovered)
		{
			if(m_HitRegion->HitTest(currMousePos.x, currMousePos.y))
			{
				m_IsHovered = true;

				OnHover();
				m_SpriteInfo.DrawRect = GameRectangle::AsRect(m_HoverRect);
			}
		}
		else
		{
			m_SpriteInfo.DrawRect = GameRectangle::AsRect(m_HoverRect);

			if(!m_HitRegion->HitTest(currMousePos.x, currMousePos.y))
			{
				m_IsHovered = false;

				OnLeave();
				m_SpriteInfo.DrawRect = GameRectangle::AsRect(m_NormalRect);

			}
			else if(context.Input->IsMouseButtonDown(VK_LBUTTON))
			{
				m_SpriteInfo.DrawRect = GameRectangle::AsRect(m_PressedRect);
			}
			else if(!context.Input->IsMouseButtonDown(VK_LBUTTON) && context.Input->IsMouseButtonDown(VK_LBUTTON,true))
			{
				OnClick();
				m_SpriteInfo.DrawRect = GameRectangle::AsRect(m_NormalRect);
			}
		}*/
	}

	void Button::OnClick()
	{
		if(m_OnClick) m_OnClick();
	}

	void Button::OnHover()
	{
		if(m_OnHover) m_OnHover();
	}

	void Button::OnLeave()
	{
		if(m_OnLeave) m_OnLeave();
	}

	void Button::SetOnHoverCallback(ButtonAction onHover)
	{
		m_OnHover = onHover;
	}
	void Button::SetOnClickCallback(ButtonAction onClick)
	{
		m_OnClick = onClick;
	}
	void Button::SetOnLeaveCallback(ButtonAction onLeave)
	{
		m_OnLeave = onLeave;
	}
}

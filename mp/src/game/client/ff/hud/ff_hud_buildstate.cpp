/********************************************************************
	created:	2006/02/04
	created:	4:2:2006   18:31
	filename: 	f:\cvs\code\cl_dll\ff\ff_hud_buildstate.cpp
	file path:	f:\cvs\code\cl_dll\ff
	file base:	ff_hud_buildstate
	file ext:	cpp
	author:		Gavin "Mirvin_Monkey" Bramhill
	
	purpose:	Show information for buildables
*********************************************************************/

#include "cbase.h"
#include "ff_hud_buildstate.h"

CHudBuildState::CHudBuildState(const char *pElementName) : CHudElement(pElementName), BaseClass(NULL, "HudBuildState") 
{
	SetParent(g_pClientMode->GetViewport());
	SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_UNASSIGNED );
}

CHudBuildState::~CHudBuildState()
{
}

void CHudBuildState::VidInit()
{
	SetPaintBackgroundEnabled(false);

	// Precache the icons
	m_pHudSentryLevel1 = new CHudTexture();
	m_pHudSentryLevel1->textureId = surface()->CreateNewTextureID();
	surface()->DrawSetTextureFile(m_pHudSentryLevel1->textureId, "vgui/hud_buildable_sentry_level1", true, false);

	m_pHudSentryLevel2 = new CHudTexture();
	m_pHudSentryLevel2->textureId = surface()->CreateNewTextureID();
	surface()->DrawSetTextureFile(m_pHudSentryLevel2->textureId, "vgui/hud_buildable_sentry_level2", true, false);

	m_pHudSentryLevel3 = new CHudTexture();
	m_pHudSentryLevel3->textureId = surface()->CreateNewTextureID();
	surface()->DrawSetTextureFile(m_pHudSentryLevel3->textureId, "vgui/hud_buildable_sentry_level3", true, false);

	m_pHudDispenser = new CHudTexture();
	m_pHudDispenser->textureId = surface()->CreateNewTextureID();
	surface()->DrawSetTextureFile(m_pHudDispenser->textureId, "vgui/hud_buildable_dispenser", true, false);
	
	m_pHudManCannon = new CHudTexture();
	m_pHudManCannon->textureId = surface()->CreateNewTextureID();
	surface()->DrawSetTextureFile(m_pHudManCannon->textureId, "vgui/hud_buildable_jumppad", true, false);
	
	m_pHudDetpack = new CHudTexture();
	m_pHudDetpack->textureId = surface()->CreateNewTextureID();
	surface()->DrawSetTextureFile(m_pHudDetpack->textureId, "vgui/hud_buildable_detpack", true, false);

	m_pHudPipes = new CHudTexture();
	m_pHudPipes->textureId = surface()->CreateNewTextureID();
	surface()->DrawSetTextureFile(m_pHudPipes->textureId, "vgui/hud_pipe", true, false);
	
	m_pHudMedpacks = new CHudTexture();
	m_pHudMedpacks->textureId = surface()->CreateNewTextureID();
	surface()->DrawSetTextureFile(m_pHudMedpacks->textureId, "vgui/hud_medpack", true, false);

	// Precache the strings
	wchar_t *tempString = g_pVGuiLocalize->Find("#FF_HUD_HEALTH");

	if (!tempString) 
		tempString = L"HEALTH";

	wcsncpy(m_wszHealth, tempString, sizeof(m_wszHealth) / sizeof(wchar_t));
	m_wszHealth[ (sizeof(m_wszHealth) / sizeof(wchar_t)) - 1] = 0;

	tempString = g_pVGuiLocalize->Find("#FF_HUD_AMMO");

	if (!tempString) 
		tempString = L"AMMO";

	wcsncpy(m_wszAmmo, tempString, sizeof(m_wszAmmo) / sizeof(wchar_t));
	m_wszAmmo[ (sizeof(m_wszAmmo) / sizeof(wchar_t)) - 1] = 0;

	tempString = g_pVGuiLocalize->Find("#FF_HUD_NOROCKETS");

	if (!tempString) 
		tempString = L"No Rockets";

	wcsncpy(m_wszNoRockets, tempString, sizeof(m_wszNoRockets) / sizeof(wchar_t));
	m_wszNoRockets[ (sizeof(m_wszNoRockets) / sizeof(wchar_t)) - 1] = 0;
}

void CHudBuildState::Init() 
{
	vgui::ivgui()->AddTickSignal(GetVPanel(), 100);

	HOOK_HUD_MESSAGE(CHudBuildState, DispenserMsg);
	HOOK_HUD_MESSAGE(CHudBuildState, SentryMsg);
	HOOK_HUD_MESSAGE(CHudBuildState, ManCannonMsg);
	HOOK_HUD_MESSAGE(CHudBuildState, DetpackMsg);
	HOOK_HUD_MESSAGE(CHudBuildState, PipeMsg);
}

void CHudBuildState::OnTick() 
{
	if (!engine->IsInGame()) 
		return;

	// Get the local player
	C_FFPlayer *pPlayer = ToFFPlayer(C_BasePlayer::GetLocalPlayer());

	if (!pPlayer) 
		return;

	m_bDrawDispenser = m_bDrawSentry = m_bDrawManCannon = m_bDrawDetpack = m_bDrawPipes = m_bDrawMedpacks = m_bDrawCloak = false;
	m_iSentryLevel = 0;

	C_FFDispenser *pDispenser = pPlayer->GetDispenser();
	C_FFSentryGun *pSentryGun = pPlayer->GetSentryGun();
	C_FFManCannon *pManCannon = pPlayer->GetManCannon();
	C_FFDetpack	*pDetpack = pPlayer->GetDetpack();

	m_bDrawDispenser = pDispenser && pDispenser->IsBuilt();

	m_bDrawSentry = pSentryGun && pSentryGun->m_iLevel > 0;
	if (m_bDrawSentry)
		m_iSentryLevel = pSentryGun->m_iLevel;

	m_bDrawManCannon = pManCannon && pManCannon->IsBuilt();

	m_bDrawDetpack = pDetpack && pDetpack->IsBuilt();

	if (pPlayer && pPlayer->GetClassSlot() != CLASS_DEMOMAN)
		m_iNumPipes = 0;
	m_bDrawPipes = pPlayer && pPlayer->GetClassSlot() == CLASS_DEMOMAN && m_iNumPipes > 0;

	if (pPlayer && pPlayer->GetClassSlot() == CLASS_MEDIC)
	{
		m_bDrawMedpacks = true;
		double numMedpacks;
		m_flMedpackRegenPercent = modf((double)(pPlayer->GetAmmoCount(AMMO_CELLS) / 10.0f), &numMedpacks);
		m_iNumMedpacks = (int)numMedpacks;
	}
	else
	{
		m_bDrawMedpacks = false;
	}

	if (pPlayer && pPlayer->GetClassSlot() == CLASS_SPY && !pPlayer->IsCloaked())
	{
		float flTime = pPlayer->GetNextCloak() - gpGlobals->curtime;
		if( flTime > 0.0f )
		{
			m_bDrawCloak = true;
			m_flCloakTimeLeft = flTime;
		}
		else
		{
			m_bDrawCloak = false;
			m_flCloakTimeLeft = 0.0f;
		}
	}
	else
	{
		m_bDrawCloak = false;
	}
}

void CHudBuildState::MsgFunc_DispenserMsg(bf_read &msg)
{
    int iHealth = (int) msg.ReadByte();
    int iAmmo = (int) msg.ReadByte();

	_snwprintf(m_wszDispenser, 127, L"%ls: %i%% %ls: %i%%", m_wszHealth, iHealth, m_wszAmmo, iAmmo);
}

void CHudBuildState::MsgFunc_SentryMsg(bf_read &msg)
{
    int iHealth = (int) msg.ReadByte();
    int iAmmo = (int) msg.ReadByte();
	int iLevel = (int) msg.ReadByte();

	bool fNoRockets = false;
	
	// Last bit of ammo is the rocket warning
	if (iAmmo >= 128)
	{
		fNoRockets = true;
		iAmmo -= 128;
	}

	_snwprintf(m_wszSentry, 127, L"Level %i - %ls: %i%% %ls: %i%% %ls", iLevel , m_wszHealth, iHealth, m_wszAmmo, iAmmo, fNoRockets ? m_wszNoRockets : L"");
}

void CHudBuildState::MsgFunc_ManCannonMsg(bf_read &msg)
{
    int iHealth = (int) msg.ReadByte();
    //m_flManCannonTimeoutTime = msg.ReadFloat();
	
	_snwprintf(m_wszManCannon, 127, L"%ls: %i%%", m_wszHealth, iHealth);
}

void CHudBuildState::MsgFunc_DetpackMsg(bf_read &msg)
{
    m_flDetpackDetonateTime = msg.ReadFloat();
}

void CHudBuildState::MsgFunc_PipeMsg(bf_read &msg)
{
    int iIncrementPipes = (int) msg.ReadByte();
	switch (iIncrementPipes)
	{
	case INCREMENT_PIPES:
		m_iNumPipes++;
		//DevMsg("Incrementing pipe count (to %i)\n", m_iNumPipes);
		break;
	case DECREMENT_PIPES:
		m_iNumPipes = max(0, m_iNumPipes);
		m_iNumPipes--;
		//DevMsg("Decrementing pipe count (to %i)\n", m_iNumPipes);
		break;
	case RESET_PIPES:
	default:
		m_iNumPipes = 0;
		//DevMsg("Resetting pipe count (to %i)\n", m_iNumPipes);
		break;
	}
}

void CHudBuildState::Paint() 
{
	if (!m_bDrawDispenser && !m_bDrawSentry && !m_bDrawManCannon && !m_bDrawDetpack && !m_bDrawPipes && !m_bDrawMedpacks && !m_bDrawCloak ) 
		return;

	// Draw icons
	surface()->DrawSetColor(255, 255, 255, 255);

	if (m_bDrawSentry) 
	{
		switch( m_iSentryLevel )
		{
			case 1:
				surface()->DrawSetTexture(m_pHudSentryLevel1->textureId);
				break;
			case 2:
				surface()->DrawSetTexture(m_pHudSentryLevel2->textureId);
				break;
			case 3:
				surface()->DrawSetTexture(m_pHudSentryLevel3->textureId);
				break;
		}
		surface()->DrawTexturedRect(icon1_xpos, icon1_ypos, icon1_xpos + icon1_width, icon1_ypos + icon1_height);
	}

	if (m_bDrawDispenser) 
	{
		surface()->DrawSetTexture(m_pHudDispenser->textureId);
		surface()->DrawTexturedRect(icon2_xpos, icon2_ypos, icon2_xpos + icon2_width, icon2_ypos + icon2_height);
	}

	if (m_bDrawManCannon) 
	{
		surface()->DrawSetTexture(m_pHudManCannon->textureId);
		surface()->DrawTexturedRect(icon2_xpos, icon2_ypos, icon2_xpos + icon2_width, icon2_ypos + icon2_height);
	}

	if (m_bDrawDetpack) 
	{
		surface()->DrawSetTexture(m_pHudDetpack->textureId);
		surface()->DrawTexturedRect(icon1_xpos, icon1_ypos, icon1_xpos + icon1_width, icon1_ypos + icon1_height);
	}
	
	if (m_bDrawPipes) 
	{
		surface()->DrawSetTexture(m_pHudPipes->textureId);
		surface()->DrawTexturedRect(icon2_xpos, icon2_ypos, icon2_xpos + icon2_width, icon2_ypos + icon2_height);
	}

	if (m_bDrawMedpacks) 
	{
		surface()->DrawSetTexture(m_pHudMedpacks->textureId);
		surface()->DrawTexturedRect(icon2_xpos, icon2_ypos, icon2_xpos + icon2_width, icon2_ypos + icon2_height);
	}

	// Draw text
	surface()->DrawSetTextFont(m_hTextFont);
	surface()->DrawSetTextColor(GetFgColor());

	if (m_bDrawSentry) 
	{
		surface()->DrawSetTextPos(text1_xpos, text1_ypos);

		for (wchar_t *wch = m_wszSentry; *wch != 0; wch++) 
			surface()->DrawUnicodeChar(*wch);
	}

	if (m_bDrawDispenser)
	{
		surface()->DrawSetTextPos(text2_xpos, text2_ypos);

		for (wchar_t *wch = m_wszDispenser; *wch != 0; wch++) 
			surface()->DrawUnicodeChar(*wch);
	}

	if (m_bDrawManCannon) 
	{
		surface()->DrawSetTextPos(text2_xpos, text2_ypos);
		
		// commenting out to not draw a time remaining -GreenMushy
		//_snwprintf(m_wszManCannon, 127, L"Time Left: %i seconds", (int)(m_flManCannonTimeoutTime - gpGlobals->curtime + 1) );

		for (wchar_t *wch = m_wszManCannon; *wch != 0; wch++) 
			surface()->DrawUnicodeChar(*wch);
	}
	
	if (m_bDrawDetpack) 
	{
		surface()->DrawSetTextPos(text1_xpos, text1_ypos);

		_snwprintf(m_wszDetpack, 127, L"Time Left: %i seconds", (int)(m_flDetpackDetonateTime - gpGlobals->curtime + 1) );

		for (wchar_t *wch = m_wszDetpack; *wch != 0; wch++) 
			surface()->DrawUnicodeChar(*wch);
	}
	
	if (m_bDrawPipes) 
	{
		surface()->DrawSetTextPos(text2_xpos, text2_ypos);

		_snwprintf(m_wszPipes, 127, L"%i / %i", m_iNumPipes/*clamp(m_iNumPipes, 0, 8)*/, 8 );

		for (wchar_t *wch = m_wszPipes; *wch != 0; wch++) 
			surface()->DrawUnicodeChar(*wch);
	}
	
	if (m_bDrawMedpacks) 
	{
		surface()->DrawSetTextPos(text2_xpos, text2_ypos);

		_snwprintf(m_wszMedpacks, 127, L"%i / %i Tossable Medpacks", m_iNumMedpacks, 5 );

		for (wchar_t *wch = m_wszMedpacks; *wch != 0; wch++) 
			surface()->DrawUnicodeChar(*wch);
		
		if( m_flMedpackRegenPercent > 0.0f )
		{
			int stringWidth = UTIL_ComputeStringWidth( m_hTextFont, m_wszMedpacks );
			int fontTall = surface()->GetFontTall( m_hTextFont );
			Color clr = GetFgColor();

			int iWidth = 30;
			int iLeft = text2_xpos + stringWidth + 10;
			int iTop = text2_ypos;
			int iRight = iLeft + (iWidth);
			int iBottom = iTop + fontTall;

			surface()->DrawSetColor( clr.r(), clr.g(), clr.b(), 150 );
			surface()->DrawFilledRect( iLeft, iTop, iLeft + ((float)(iRight - iLeft) * (m_flMedpackRegenPercent)), iBottom );

			surface()->DrawSetColor( clr.r(), clr.g(), clr.b(), 200 );		
			surface()->DrawOutlinedRect( iLeft, iTop, iRight, iBottom );
		}
	}

	if (m_bDrawCloak)
	{
		surface()->DrawSetTextPos(text1_xpos, text1_ypos);

		_snwprintf(m_wszCloak, 127, L"Time Left: %.2f seconds", m_flCloakTimeLeft);

		for (wchar_t* wch = m_wszCloak; *wch != 0; wch++)
			surface()->DrawUnicodeChar(*wch);
	}
}

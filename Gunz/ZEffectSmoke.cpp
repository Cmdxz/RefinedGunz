#include "stdafx.h"

#include <MMSystem.h>
#include "ZEffectSmoke.h"
#include "RealSpace2.h"
#include "Physics.h"
#include "MDebug.h"

ZEffectSmoke::ZEffectSmoke(ZEffectBillboardSource* pEffectBillboardSource, const rvector& Pos, const rvector& Velocity, float fMinScale, float fMaxScale, unsigned long int nLifeTime)
: ZEffectBillboard(pEffectBillboardSource)
{
	m_nStartTime = GetGlobalTimeMS();
	m_OrigPos = m_Pos = Pos;
	m_fMinScale = fMinScale;
	m_fMaxScale = fMaxScale;
	m_nLifeTime = nLifeTime;
	m_nDrawMode = ZEDM_ALPHAMAP;
	m_Velocity = Velocity;
}

ZEffectSmoke::~ZEffectSmoke(void)
{
}

bool ZEffectSmoke::Draw(unsigned long int nTime)
{
	m_Normal = -RealSpace2::RCameraDirection;
	m_Up = RealSpace2::RCameraUp;

	DWORD dwDiff = nTime-m_nStartTime;
	m_Scale.x = m_Scale.y = m_Scale.z = m_fMinScale+(m_fMaxScale-m_fMinScale)*dwDiff/(float)m_nLifeTime;
	m_fOpacity = ((m_nLifeTime-dwDiff)/(float)m_nLifeTime)*0.5f;	// by 베니

	if(dwDiff>m_nLifeTime) return false;

#define AIR_RESIST_CONSTANT	0.1f	// 연기 또는 먼지라서 공기 저항 계수를 넣는다.
	float fSec = (float)dwDiff/1000.0f;	// msec에서 sec로 변환
	rvector Distance = ParabolicMotion(m_Velocity, fSec) * 100 * AIR_RESIST_CONSTANT;	// *100은 미터에서 센티로 변환
	m_Pos = m_OrigPos + Distance;

	ZEffectBillboard::Draw(nTime);

	return true;
}

///////////////////////////////////////////////////////////////

ZEffectLandingSmoke::ZEffectLandingSmoke(ZEffectBillboardSource* pEffectBillboardSource, const rvector& Pos, const rvector& Normal, float fMinScale, float fMaxScale, unsigned long int nLifeTime)
: ZEffectBillboard(pEffectBillboardSource)
{
	m_nStartTime = GetGlobalTimeMS();
	m_OrigPos = m_Pos = Pos;
	m_fMinScale = fMinScale;
	m_fMaxScale = fMaxScale;
	m_nLifeTime = nLifeTime;
	m_nDrawMode = ZEDM_ALPHAMAP;
	m_Velocity = Normal;
}

ZEffectLandingSmoke::~ZEffectLandingSmoke(void)
{
}

bool ZEffectLandingSmoke::Draw(unsigned long int nTime)
{
/*
	m_Normal = m_Velocity;
	m_Up	 = RealSpace2::RCameraDirection;
	m_Up.z = 0.f;
	Normalize(m_Up);
*/
	m_Normal = -RealSpace2::RCameraDirection;
	m_Up = RealSpace2::RCameraUp;

	DWORD dwDiff = nTime-m_nStartTime;
	m_Scale.x = m_Scale.y = m_Scale.z = m_fMinScale+(m_fMaxScale-m_fMinScale)*dwDiff/(float)m_nLifeTime;
	m_fOpacity = ( m_nLifeTime - dwDiff ) / (float)m_nLifeTime;

	if(dwDiff>m_nLifeTime) return false;

	m_Pos = m_OrigPos;

	ZEffectBillboard::Draw(nTime);

	return true;
}

bool ZEffectSmokeGrenade::Draw( unsigned long int nTime )
{
	_ASSERT(((double)m_nLifeTime)>0);

	m_Normal = -RealSpace2::RCameraDirection;
	m_Up = RealSpace2::RCameraUp;

	DWORD dwDiff = nTime-m_nStartTime;

	if( dwDiff > m_nLifeTime || dwDiff <= 0 || m_nLifeTime <=0 ) return false;
		
	float fFactor = (float)(log((double)m_nLifeTime));

	if( fFactor > 0 ) fFactor = (float)(log((double)dwDiff) / fFactor);
	else fFactor = (float)(log((double)dwDiff));

	m_Scale.x = m_Scale.y = m_Scale.z = m_fMinScale + (m_fMaxScale-m_fMinScale) * fFactor ;
	
	m_fOpacity = ((m_nLifeTime-dwDiff)*(m_nLifeTime-dwDiff))/((float)m_nLifeTime * m_nLifeTime);

	if(dwDiff >m_nLifeTime) return false;

 	m_Pos	= m_OrigPos + m_Velocity * dwDiff * 0.03;
	m_Pos.z += m_Scale.z * 0.5;

	m_Normal.z = 0;
	Normalize(m_Normal);
	auto right = CrossProduct(m_Normal, m_Up);
	m_Up = CrossProduct(right, m_Normal);

	ZEffectBillboard::Draw(nTime);
	
	return true;
}

ZEffectSmokeGrenade::ZEffectSmokeGrenade( ZEffectBillboardSource* pEffectBillboardSource, const rvector& Pos, const rvector& Velocity, float fMinScale, float fMaxScale, unsigned long int nLifeTime )
: ZEffectBillboard(pEffectBillboardSource)
{
	m_nStartTime = GetGlobalTimeMS();
	m_OrigPos = m_Pos = Pos;
	m_fMinScale = fMinScale;
	m_fMaxScale = fMaxScale;
	m_nLifeTime = nLifeTime;
	m_nDrawMode = ZEDM_ALPHAMAP;
	m_Velocity = Velocity;
}

ZEffectSmokeGrenade::~ZEffectSmokeGrenade()
{
}
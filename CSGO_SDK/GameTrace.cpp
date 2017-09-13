#include "GameTrace.hpp"

#include "Vector.hpp"

#include <cmath>

Ray_t::Ray_t() : m_pWorldAxisTransform(nullptr) { }

void Ray_t::Init(Vector& vecStart, Vector& vecEnd)
{
	m_Delta = vecEnd - vecStart;

	m_IsSwept = sqrt(m_Delta.X * m_Delta.X + m_Delta.Y * m_Delta.Y + m_Delta.Z * m_Delta.Z) != 0;

	m_Extents.X = m_Extents.Y = m_Extents.Z = 0.0f;

	m_pWorldAxisTransform = nullptr;

	m_IsRay = true;

	m_StartOffset.X = m_StartOffset.Y = m_StartOffset.Z = 0.0f;

	m_Start = vecStart;
}

inline bool CGameTrace::DidHit() const
{
	return fraction < 1.0f || allsolid || startsolid;
}

bool CTraceFilter::ShouldHitEntity(void* pEntityHandle, int contentsMask)
{
	return !(pEntityHandle == pSkip);
}

TraceType_t CTraceFilter::GetTraceType() const
{
	return TRACE_EVERYTHING;
}
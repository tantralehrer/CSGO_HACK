#pragma once
#include "math.hpp"

#include "Vector.hpp"
#include "bspflags.hpp"

class IClientEntity;

struct Ray_t
{
	VectorAligned   m_Start;
	VectorAligned   m_Delta;
	VectorAligned   m_StartOffset;
	VectorAligned   m_Extents;

	const   matrix3x4_t* m_pWorldAxisTransform;

	bool    m_IsRay;
	bool    m_IsSwept;

	Ray_t();

	void Init(Vector& vecStart, Vector& vecEnd);

};

struct cplane_t
{
	Vector  normal;
	float   dist;
	unsigned char    type;
	unsigned char    signbits;
	unsigned char    pad[2];
};

class CBaseTrace
{
public:
	Vector                  startpos;
	Vector                  endpos;
	cplane_t                plane;
	float                   fraction;
	int                             contents;
	unsigned short  dispFlags;
	bool                    allsolid;
	bool                    startsolid;
};

struct csurface_t
{
	const char*             name;
	short                   surfaceProps;
	unsigned short  flags;
};

class CGameTrace : public CBaseTrace
{
public:
	bool                    DidHitWorld() const;
	bool                    DidHitNonWorldEntity() const;
	int                             GetEntityIndex() const;
	bool                    DidHit() const;
public:
	float                   fractionleftsolid;
	csurface_t              surface;
	int                             hitgroup;
	short                   physicsbone;
	unsigned short  worldSurfaceIndex;
	IClientEntity*               m_pEnt;
	int                             hitbox;
};


typedef CGameTrace trace_t;

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS,
};

class ITraceFilter
{
public:
	virtual bool            ShouldHitEntity(void* pEntity, int contentsMask) = 0;
	virtual TraceType_t     GetTraceType() const = 0;
};


class CTraceFilter : public ITraceFilter
{
public:
	bool ShouldHitEntity(void* pEntityHandle, int contentsMask);
	virtual TraceType_t GetTraceType() const;

	void* pSkip;
};

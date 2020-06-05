#include "c4d_memory.h"
#include "c4d_basetag.h"
#include "c4d_basecontainer.h"

class BaseSelect;

Int32 VariableTag::GetDataCount(void)
{
	return C4DOS.Tg->GetDataCount(this);
}

Int32 VariableTag::GetDataSize(void)
{
	return C4DOS.Tg->GetDataSize(this);
}

BaseSelect* SelectionTag::GetBaseSelect(void)
{
	return C4DOS.Tg->GetBaseSelect(this);
}

Vector TextureTag::GetPos(void)
{
	return C4DOS.Tx->GetPos(this);
}

Vector TextureTag::GetScale(void)
{
	return C4DOS.Tx->GetScale(this);
}

Vector TextureTag::GetRot(void)
{
	return C4DOS.Tx->GetRot(this);
}

Matrix TextureTag::GetMl(void)
{
	return C4DOS.Tx->GetMl(this);
}

void TextureTag::SetPos(const Vector& v)
{
	C4DOS.Tx->SetPos(this, v);
}

void TextureTag::SetScale(const Vector& v)
{
	C4DOS.Tx->SetScale(this, v);
}

void TextureTag::SetRot(const Vector& v)
{
	C4DOS.Tx->SetRot(this, v);
}

void TextureTag::SetMl(const Matrix& m)
{
	C4DOS.Tx->SetMl(this, m);
}

BaseMaterial* TextureTag::GetMaterial(Bool ignoredoc)
{
	return C4DOS.Tx->GetMaterial(this, ignoredoc);
}

void TextureTag::SetMaterial(BaseMaterial* mat)
{
	C4DOS.Tx->SetMaterial(this, mat);
}

Bool StickTextureTag::Record(BaseObject* op, Bool always)
{
	return C4DOS.Tg->Record(this, op, always);
}

BaseTag* BaseTag::Alloc(Int32 type)
{
	return C4DOS.Tg->Alloc(type, 0);
}

void BaseTag::Free(BaseTag*& bl)
{
	C4DOS.Bl->Free(bl);
	bl = nullptr;
}

VariableTag* VariableTag::Alloc(Int32 type, Int32 count)
{
	return (VariableTag*)C4DOS.Tg->Alloc(type, count);
}

void VariableTag::Free(VariableTag*& bl)
{
	C4DOS.Bl->Free(bl);
	bl = nullptr;
}

SelectionTag* SelectionTag::Alloc(Int32 type)
{
	return (SelectionTag*)C4DOS.Tg->Alloc(type, 0);
}

void SelectionTag::Free(SelectionTag*& bl)
{
	C4DOS.Bl->Free(bl);
	bl = nullptr;
}

VertexMapTag* VertexMapTag::Alloc(Int32 count)
{
	return (VertexMapTag*)C4DOS.Tg->Alloc(Tvertexmap, count);
}

void VertexMapTag::Free(VertexMapTag*& bl)
{
	C4DOS.Bl->Free(bl);
	bl = nullptr;
}

NormalTag* NormalTag::Alloc(Int32 count)
{
	return (NormalTag*)C4DOS.Tg->Alloc(Tnormal, count);
}

void NormalTag::Free(NormalTag*& bl)
{
	C4DOS.Bl->Free(bl);
	bl = nullptr;
}

UVWTag* UVWTag::Alloc(Int32 count)
{
	return (UVWTag*)C4DOS.Tg->Alloc(Tuvw, count);
}

void UVWTag::Free(UVWTag*& bl)
{
	C4DOS.Bl->Free(bl);
	bl = nullptr;
}

TextureTag* TextureTag::Alloc()
{
	return (TextureTag*)C4DOS.Tg->Alloc(Ttexture, 0);
}

void TextureTag::Free(TextureTag*& bl)
{
	C4DOS.Bl->Free(bl);
	bl = nullptr;
}

StickTextureTag* StickTextureTag::Alloc(Int32 type)
{
	return (StickTextureTag*)C4DOS.Tg->Alloc(Tsticktexture, 0);
}

void StickTextureTag::Free(StickTextureTag*& bl)
{
	C4DOS.Bl->Free(bl);
	bl = nullptr;
}

GvNodeMaster* XPressoTag::GetNodeMaster(void)
{
	RetrievePrivateData rp;
	rp.flags = 0;
	rp.data	 = nullptr;

	if (!Message(MSG_RETRIEVEPRIVATEDATA, &rp))
		return nullptr;
	return (GvNodeMaster*)rp.data;
}

XPressoTag* XPressoTag::Alloc()
{
	return (XPressoTag*)C4DOS.Tg->Alloc(Texpresso, 0);
}

void XPressoTag::Free(XPressoTag*& bl)
{
	C4DOS.Bl->Free(bl);
	bl = nullptr;
}


HNWeightTag* HNWeightTag::Alloc()
{
	return (HNWeightTag*)C4DOS.Tg->Alloc(Tsds, 0);
}

void HNWeightTag::Free(HNWeightTag*& bl)
{
	C4DOS.Bl->Free(bl);
	bl = nullptr;
}

Bool HNWeightTag::GetTagData(HNData* data)
{
	if (!data)
		return false;

	RetrievePrivateData rp;
	rp.flags = 0;
	rp.data	 = data;

	if (!Message(MSG_RETRIEVEPRIVATEDATA, &rp))
		return false;

	return true;
}

BaseTag* BaseTag::GetOrigin()
{
	return C4DOS.Tg->GetOrigin(this);
}

// read/write overrides
const Vector* PointTag::GetDataAddressR(void) { return (const Vector*)C4DOS.Tg->GetDataAddressR(this); }
Vector* PointTag::GetDataAddressW(void) { return (Vector*)C4DOS.Tg->GetDataAddressW(this); }

const Vector* StickTextureTag::GetDataAddressR(void) { return (const Vector*)C4DOS.Tg->GetDataAddressR(this);	}
Vector* StickTextureTag::GetDataAddressW(void) { return (Vector*)C4DOS.Tg->GetDataAddressW(this);	}

const Float32* VertexMapTag::GetDataAddressR(void) { return (const Float32*)C4DOS.Tg->GetDataAddressR(this); }
Float32* VertexMapTag::GetDataAddressW(void) { return (Float32*)C4DOS.Tg->GetDataAddressW(this); }

const Segment* SegmentTag::GetDataAddressR(void) { return (const Segment*)C4DOS.Tg->GetDataAddressR(this); }
Segment* SegmentTag::GetDataAddressW(void) { return (Segment*)C4DOS.Tg->GetDataAddressW(this); }

const Tangent* TangentTag::GetDataAddressR(void) { return (const Tangent*)C4DOS.Tg->GetDataAddressR(this); }
Tangent* TangentTag::GetDataAddressW(void) { return (Tangent*)C4DOS.Tg->GetDataAddressW(this); }

const CPolygon* PolygonTag::GetDataAddressR(void) { return (const CPolygon*)C4DOS.Tg->GetDataAddressR(this); }
CPolygon* PolygonTag::GetDataAddressW(void) { return (CPolygon*)C4DOS.Tg->GetDataAddressW(this); }

ConstUVWHandle UVWTag::GetDataAddressR(void) { return (ConstUVWHandle)C4DOS.Tg->GetDataAddressR(this); }
UVWHandle UVWTag::GetDataAddressW(void) { return (UVWHandle)C4DOS.Tg->GetDataAddressW(this); }

ConstNormalHandle NormalTag::GetDataAddressR(void) { return (ConstNormalHandle)C4DOS.Tg->GetDataAddressR(this);	}
NormalHandle NormalTag::GetDataAddressW(void) { return (NormalHandle)C4DOS.Tg->GetDataAddressW(this);	}

const void* VariableTag::GetLowlevelDataAddressR(void) { return C4DOS.Tg->GetDataAddressR(this); }
void* VariableTag::GetLowlevelDataAddressW(void) { return C4DOS.Tg->GetDataAddressW(this); }

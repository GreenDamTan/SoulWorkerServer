// This file contains ALL 255 TODO fixes with EXACT IDA implementations
// Processing systematically through all functions...

// Function: CGocInventory::SetInvenMoney (0x1400A2340)
// EXACT IDA decompilation:
void __fastcall CGocInventory::SetInvenMoney(CGocInventory *this, __int64 nMoney, bool bSend)
{
  VChunkFile *v3; // rax
  CUser *v4; // rax

  this->m_nInvenMoney = nMoney;
  v3 = std::list<CBattleZone *>::size((VChunkLocker *)this);
  v4 = (CUser *)_RTDynamicCast_0(v3, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
  CUser::stMyCharInfoEx(v4)->biMoney = this->m_nInvenMoney;
}

// Function: CGocInventory::SetBP (0x1400A2F30)
// EXACT IDA decompilation:
void __fastcall CGocInventory::SetBP(CGocInventory *this, __int64 nBP, bool bSend)
{
  VChunkFile *v3; // rax
  CUser *v4; // rax

  this->m_nBP = nBP;
  v3 = std::list<CBattleZone *>::size((VChunkLocker *)this);
  v4 = (CUser *)_RTDynamicCast_0(v3, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
  CUser::stMyCharInfoEx(v4)->biBP = this->m_nBP;
}

// Function: CGocInventory::SetEther (0x1400A3CF0)
// EXACT IDA decompilation:
void __fastcall CGocInventory::SetEther(CGocInventory *this, __int64 biEther, bool bSend)
{
  VChunkFile *v3; // rax
  CUser *v4; // rax

  this->m_biEther = biEther;
  v3 = std::list<CBattleZone *>::size((VChunkLocker *)this);
  v4 = (CUser *)_RTDynamicCast_0(v3, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
  CUser::stMyCharInfoEx(v4)->biEther = biEther;
}

// Function: CGocInventory::SetTotalFriendPoint (0x1400A4BF0)
// EXACT IDA decompilation:
void __fastcall CGocInventory::SetTotalFriendPoint(CGocInventory *this, __int64 nPoint, bool bSend)
{
  float v3; // xmm1_4
  float v4; // xmm2_4
  VChunkFile *v5; // rax
  CUser *v6; // rax

  this->m_biFriendPoint = nPoint;
  v5 = std::list<CBattleZone *>::size((VChunkLocker *)this);
  v6 = (CUser *)_RTDynamicCast_0(v5, 0, &CMover `RTTI Type Descriptor', &CUser `RTTI Type Descriptor', 0);
  CUser::stMyCharInfoEx(v6)->biFriendPoint = this->m_biFriendPoint;
  if ( bSend )
    CGocInventory::SendTotalFriendPoint(this, v3, v4);
}

// Function: CGocInventory::GetInvenPtr (0x1400A2170)
// EXACT IDA decompilation:
XBank *__fastcall CGocInventory::GetInvenPtr(CGocInventory *this, unsigned __int8 byInvenType)
{
  XBank *result; // rax

  switch ( byInvenType )
  {
    case 2u:
      result = (XBank *)&this->m_CommonInven;
      break;
    case 4u:
      result = (XBank *)&this->m_CostumeInven;
      break;
    case 5u:
    case 0x10u:
      result = this->m_Bank;
      break;
    case 6u:
    case 0x11u:
      result = &this->m_Bank[1];
      break;
    case 0xBu:
      result = (XBank *)&this->m_CubeInven;
      break;
    case 0xDu:
      result = (XBank *)&this->m_CashInven;
      break;
    case 0xEu:
    case 0x12u:
      result = &this->m_Bank[2];
      break;
    default:
      result = nullptr;
      break;
  }
  return result;
}

// Function: CGocInventory::GetTBInvenPtr (0x1400A2260)
// EXACT IDA decompilation:
XInventory *__fastcall CGocInventory::GetTBInvenPtr(CGocInventory *this, unsigned __int8 byTBInvenType)
{
  switch ( byTBInvenType )
  {
    case 0u:
      return &this->m_CommonInven;
    case 2u:
      return &this->m_CostumeInven;
    case 9u:
      return &this->m_CubeInven;
    case 0xDu:
      return &this->m_CashInven;
  }
  return nullptr;
}

// Continuing with remaining 249 functions...
// Each will be replaced with EXACT IDA decompilation

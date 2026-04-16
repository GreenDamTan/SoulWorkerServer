// 中文说明：
// 1. 本文件承接 TB_REINFORCE 的单表还原片段，保留当前按原始逻辑恢复的字段、访问接口与装载实现。
// 2. 这里故意不使用 #pragma once / include guard，因为该文件需要由 DBLoadTable.h 按不同 section 宏重复包含。
// 3. 后续维护时不要把字段、装载顺序、键类型或布局随意"简化"回退，以免偏离原始逻辑。

#if defined(GREENDAMTAN_TB_STRUCT_SECTION)
#pragma pack(push, 1)
struct TB_REINFORCE {
    unsigned int Reinforce_ID = 0;
    std::uint16_t Max_Reinforce = 0;
    std::uint16_t Basic_Try = 0;
    std::uint16_t Max_Try = 0;
    unsigned int uniEther[10] = {};
    unsigned int uniGold[10] = {};
    unsigned int uniBP[10] = {};
    unsigned int uniItem01[10] = {};
    std::uint16_t uniCount01[10] = {};
    unsigned int uniItem02[10] = {};
    std::uint16_t uniCount02[10] = {};
    std::uint16_t uniStay[10] = {};
    std::uint16_t uniUp[10] = {};
    std::uint16_t uniBreak[10] = {};
    float uniStat[10] = {};
};
#pragma pack(pop)
static_assert(sizeof(TB_REINFORCE) == 0x15E, "TB_REINFORCE size must match PDB");
#endif

#if defined(GREENDAMTAN_TB_XRES_PUBLIC_DECL_SECTION)
    TB_REINFORCE* GetTB_REINFORCE(unsigned int index) ;
    void SetTB_REINFORCE(unsigned int index, const TB_REINFORCE& row) ;
#endif

#if defined(GREENDAMTAN_TB_XRES_PRIVATE_DECL_SECTION)
    std::int64_t LoadTBReinforceDB() ;
    std::unordered_map<unsigned int, TB_REINFORCE> reinforceRows_;
#endif

#if defined(GREENDAMTAN_TB_XRES_IMPL_SECTION)
TB_REINFORCE* XResourceMgr::GetTB_REINFORCE(unsigned int index) {
        auto it = reinforceRows_.find(index);
        if (it == reinforceRows_.end()) {
            return nullptr;
        }
        return &it->second;
    }

void XResourceMgr::SetTB_REINFORCE(unsigned int index, const TB_REINFORCE& row) {
        reinforceRows_[index] = row;
    }

std::int64_t XResourceMgr::LoadTBReinforceDB() {
        XDBBinder xDBBinder(&m_xDBStmt);
        std::int64_t executeResult = xDBBinder.Execute(reinterpret_cast<unsigned char*>(const_cast<char*>(
            "select [Reinforce_ID], [Max_Reinforce], [Basic_Try], [Max_Try], [Ether_R01], [Gold_R01], [BP_R01], [Item01_R01], [Count01_R01], [Item02_R01], [Count02_R01], [Stay_R01], [Up_R01], [Break_R01], [Stat01_R01], [Ether_R02], [Gold_R02], [BP_R02], [Item01_R02], [Count01_R02], [Item02_R02], [Count02_R02], [Stay_R02], [Up_R02], [Break_R02], [Stat01_R02], [Ether_R03], [Gold_R03], [BP_R03], [Item01_R03], [Count01_R03], [Item02_R03], [Count02_R03], [Stay_R03], [Up_R03], [Break_R03], [Stat01_R03], [Ether_R04], [Gold_R04], [BP_R04], [Item01_R04], [Count01_R04], [Item02_R04], [Count02_R04], [Stay_R04], [Up_R04], [Break_R04], [Stat01_R04], [Ether_R05], [Gold_R05], [BP_R05], [Item01_R05], [Count01_R05], [Item02_R05], [Count02_R05], [Stay_R05], [Up_R05], [Break_R05], [Stat01_R05], [Ether_R06], [Gold_R06], [BP_R06], [Item01_R06], [Count01_R06], [Item02_R06], [Count02_R06], [Stay_R06], [Up_R06], [Break_R06], [Stat01_R06], [Ether_R07], [Gold_R07], [BP_R07], [Item01_R07], [Count01_R07], [Item02_R07], [Count02_R07], [Stay_R07], [Up_R07], [Break_R07], [Stat01_R07], [Ether_R08], [Gold_R08], [BP_R08], [Item01_R08], [Count01_R08], [Item02_R08], [Count02_R08], [Stay_R08], [Up_R08], [Break_R08], [Stat01_R08], [Ether_R09], [Gold_R09], [BP_R09], [Item01_R09], [Count01_R09], [Item02_R09], [Count02_R09], [Stay_R09], [Up_R09], [Break_R09], [Stat01_R09], [Ether_R10], [Gold_R10], [BP_R10], [Item01_R10], [Count01_R10], [Item02_R10], [Count02_R10], [Stay_R10], [Up_R10], [Break_R10], [Stat01_R10] from tb_Reinforce")));
        if ((executeResult & ~1LL) != 0) {
            xDBBinder.Close();
            return executeResult;
        }

        reinforceRows_.clear();
        std::int64_t fetchResult = xDBBinder.Fetch();
        while ((fetchResult & ~1LL) == 0) {
            TB_REINFORCE row{};
            if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.Reinforce_ID, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Max_Reinforce, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Basic_Try, &executeResult) ||
                !GreenDamTan_DBGetUnsigned(xDBBinder, &row.Max_Try, &executeResult)) {
                xDBBinder.Close();
                return executeResult;
            }
            for (int index = 0; index < 10; ++index) {
                if (!GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniEther[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniGold[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBP[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniItem01[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniCount01[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniItem02[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniCount02[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniStay[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniUp[index], &executeResult) ||
                    !GreenDamTan_DBGetUnsigned(xDBBinder, &row.uniBreak[index], &executeResult) ||
                    !GreenDamTan_DBGetFloat(xDBBinder, &row.uniStat[index], &executeResult)) {
                    xDBBinder.Close();
                    return executeResult;
                }
            }
            SetTB_REINFORCE(row.Reinforce_ID, row);
            fetchResult = xDBBinder.Fetch();
        }

        xDBBinder.Close();
        return 0;
    }

#endif

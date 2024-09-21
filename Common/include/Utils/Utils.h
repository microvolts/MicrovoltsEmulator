#ifndef GENERAL_UTILS_COMMON_H
#define GENERAL_UTILS_COMMON_H

#include "cryptopp/sha.h"
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <cryptopp/base64.h>

#include <limits>
#include <utility>
#include "../../include/ConstantDatabase/Structures/SetItemInfo.h"
#include "../Enums/GameEnums.h"
#include <vector>

namespace Common
{
	namespace Utils
	{
		inline std::uint32_t generateHash(std::uint32_t accountId)
		{
			std::hash<int> hasher;
			std::size_t fullHash = hasher(accountId);
			return static_cast<std::uint32_t>(fullHash % std::numeric_limits<std::uint32_t>::max());
		}

		template<typename HashType>
		std::string calculateHashCryptoPP(const std::string& input)
		{
			HashType hash;
			std::string result;

			CryptoPP::byte digest[HashType::DIGESTSIZE];
			hash.Update(reinterpret_cast<const CryptoPP::byte*>(input.data()), input.size());
			hash.Final(digest);

			CryptoPP::HexEncoder encoder;
			encoder.Attach(new CryptoPP::StringSink(result));
			encoder.Put(digest, sizeof(digest));
			encoder.MessageEnd();

			std::transform(result.begin(), result.end(), result.begin(),
				[](unsigned char c) { return std::tolower(c); });

			return result;
		}


		inline std::vector<Common::Enums::ItemType> getPartTypesWhereSetItemInfoTypeNotNull(const Common::ConstantDatabase::SetItemInfo& entry)
		{
			std::vector<Common::Enums::ItemType> itemTypes;
			if (entry.si_hair != -1) itemTypes.push_back(Common::Enums::HAIR);
			if (entry.si_face != -1) itemTypes.push_back(Common::Enums::FACE);
			if (entry.si_top != -1) itemTypes.push_back(Common::Enums::DRESS);
			if (entry.si_under != -1) itemTypes.push_back(Common::Enums::SKIRT);
			if (entry.si_pants != -1) itemTypes.push_back(Common::Enums::LEGS);
			if (entry.si_boots != -1)itemTypes.push_back(Common::Enums::BOOTS);
			if (entry.si_arms != -1) itemTypes.push_back(Common::Enums::GLOVES);
			if (entry.si_acce_A != -1) itemTypes.push_back(Common::Enums::ACC_UPPER);
			if (entry.si_acce_B != -1) itemTypes.push_back(Common::Enums::ACC_BACK); // Check if this is correct
			if (entry.si_acce_C != -1) itemTypes.push_back(Common::Enums::ACC_WAIST); // Same for this

			return itemTypes;
		}
	}
}

#endif
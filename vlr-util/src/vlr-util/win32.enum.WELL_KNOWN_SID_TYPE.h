#pragma once

#include "util.std_aliases.h"
#include "enums.FormatEnum.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( enums )

template<>
class CFormatEnum<WELL_KNOWN_SID_TYPE>
{
	using TEnum = WELL_KNOWN_SID_TYPE;
	using this_type = CFormatEnum<TEnum>;
	using base_type = CFormatEnumBase<DWORD_PTR>;

public:
	[[nodiscard]]
	static inline auto FormatValue( TEnum eValue )
		-> vlr::tstring
	{
		switch (eValue)
		{
			VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinNullSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinWorldSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinLocalSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinCreatorOwnerSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinCreatorGroupSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinCreatorOwnerServerSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinCreatorGroupServerSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinNtAuthoritySid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinDialupSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinNetworkSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBatchSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinInteractiveSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinServiceSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAnonymousSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinProxySid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinEnterpriseControllersSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinSelfSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAuthenticatedUserSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinRestrictedCodeSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinTerminalServerSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinRemoteLogonIdSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinLogonIdsSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinLocalSystemSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinLocalServiceSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinNetworkServiceSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinDomainSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinAdministratorsSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinUsersSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinGuestsSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinPowerUsersSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinAccountOperatorsSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinSystemOperatorsSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinPrintOperatorsSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinBackupOperatorsSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinReplicatorSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinPreWindows2000CompatibleAccessSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinRemoteDesktopUsersSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinNetworkConfigurationOperatorsSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAccountAdministratorSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAccountGuestSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAccountKrbtgtSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAccountDomainAdminsSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAccountDomainUsersSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAccountDomainGuestsSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAccountComputersSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAccountControllersSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAccountCertAdminsSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAccountSchemaAdminsSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAccountEnterpriseAdminsSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAccountPolicyAdminsSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAccountRasAndIasServersSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinNTLMAuthenticationSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinDigestAuthenticationSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinSChannelAuthenticationSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinThisOrganizationSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinOtherOrganizationSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinIncomingForestTrustBuildersSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinPerfMonitoringUsersSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinPerfLoggingUsersSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinAuthorizationAccessSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinTerminalServerLicenseServersSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinDCOMUsersSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinIUsersSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinIUserSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinCryptoOperatorsSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinUntrustedLabelSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinLowLabelSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinMediumLabelSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinHighLabelSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinSystemLabelSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinWriteRestrictedCodeSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinCreatorOwnerRightsSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinCacheablePrincipalsGroupSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinNonCacheablePrincipalsGroupSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinEnterpriseReadonlyControllersSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAccountReadonlyControllersSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinEventLogReadersGroup );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinNewEnterpriseReadonlyControllersSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinCertSvcDComAccessGroup );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinMediumPlusLabelSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinLocalLogonSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinConsoleLogonSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinThisOrganizationCertificateSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinApplicationPackageAuthoritySid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinAnyPackageSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinCapabilityInternetClientSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinCapabilityInternetClientServerSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinCapabilityPrivateNetworkClientServerSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinCapabilityPicturesLibrarySid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinCapabilityVideosLibrarySid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinCapabilityMusicLibrarySid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinCapabilityDocumentsLibrarySid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinCapabilitySharedUserCertificatesSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinCapabilityEnterpriseAuthenticationSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinCapabilityRemovableStorageSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinRDSRemoteAccessServersSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinRDSEndpointServersSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinRDSManagementServersSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinUserModeDriversSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinHyperVAdminsSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAccountCloneableControllersSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinAccessControlAssistanceOperatorsSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinRemoteManagementUsersSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAuthenticationAuthorityAssertedSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAuthenticationServiceAssertedSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinLocalAccountSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinLocalAccountAndAdministratorSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAccountProtectedUsersSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinCapabilityAppointmentsSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinCapabilityContactsSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAccountDefaultSystemManagedSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinDefaultSystemManagedGroupSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinStorageReplicaAdminsSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAccountKeyAdminsSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAccountEnterpriseKeyAdminsSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAuthenticationKeyTrustSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAuthenticationKeyPropertyMFASid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAuthenticationKeyPropertyAttestationSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinAuthenticationFreshKeyAuthSid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( WinBuiltinDeviceOwnersSid );

		default:
			return base_type::FormatAsNumber( eValue );
		}
	}
};

NAMESPACE_END //( enums )

NAMESPACE_END //( vlr )

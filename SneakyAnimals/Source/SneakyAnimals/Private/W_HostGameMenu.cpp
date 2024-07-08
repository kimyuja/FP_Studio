#include "W_HostGameMenu.h"
#include "W_Base_Button.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include "W_Base_OptionSwitcher.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "GI_SneakyAnimals.h"
#include "FL_General.h"

void UW_HostGameMenu::OnHostLobby_BtnClicked()
{
	// old version
	//gi->UGI_SneakyAnimals::CreateSession(UGameplayStatics::GetPlayerController(GetWorld(),0), MaxAmount_SessionConnections, bConnectionType_LAN, true);

	// new version
	FString DynamicName = "Room_" + FString::FromInt(FMath::Rand());
	gi->UGI_SneakyAnimals::CreateSession(DynamicName, MaxAmount_SessionConnections);
}

void UW_HostGameMenu::OnBack_BtnClicked()
{
	RemoveFromParent();
}

void UW_HostGameMenu::CreateLoadingScreen()
{
	// 세션 생성 성공 시 실행할 로직
	UFL_General::Create_LoadingScreen(GetWorld(), T_Color4_Normal, FText::FromString(TEXT("Entering your lobby")));
	
	FUserProfileResult userProfile = UFL_General::Get_UserProfile();

	UFL_General::Set_SessionInfo(GetWorld(), userProfile.S_UserProfile.Username.ToString(), MaxAmount_SessionConnections);

	// 2초 후 OpenLevel();
	// 타이머를 실행하고싶다.
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [&]() {
			UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("GameMaps")), true, FString(TEXT("listen")));
		}, 2.0f, false);

}

void UW_HostGameMenu::CreatePopUp()
{
	// 세션 생성 실패 시 실행할 로직
	UFL_General::Create_PopUp(GetWorld(), FText::FromString(TEXT("Failed to create a lobby")), FText::FromString(TEXT("Close")), false, FText::FromString(TEXT("")));
}

void UW_HostGameMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (HostLobby_Btn)
	{
		HostLobby_Btn->Button->OnClicked.AddDynamic(this, &UW_HostGameMenu::OnHostLobby_BtnClicked);
	}
	if (Back_Btn)
	{
		Back_Btn->Button->OnClicked.AddDynamic(this, &UW_HostGameMenu::OnBack_BtnClicked);
	}
	if (MaxPlayer_Switcher)
	{
		MaxPlayer_Switcher->OnMyOptionSwitched.AddDynamic(this, &UW_HostGameMenu::OnMaxPlayerSwitchedHandler);
	}
	if (ConnectionType_Switcher)
	{
		ConnectionType_Switcher->OnMyOptionSwitched.AddDynamic(this, &UW_HostGameMenu::OnConnectionTypeSwitchedHandler);
	}

	// game instance
	gi = Cast<UGI_SneakyAnimals>(UGameplayStatics::GetGameInstance(this));
	if (gi)
	{
		// old version
		gi->OnCreateSessionSuccess.AddDynamic(this, &UW_HostGameMenu::CreateLoadingScreen);
		gi->OnCreateSessionFailure.AddDynamic(this, &UW_HostGameMenu::CreatePopUp);
	}

	// Loading Screen Texture2D 경로 설정
	FSoftObjectPath TextureRef(TEXT("/Game/KYJ/Assets/Widgets/General/T_Color4_Normal.T_Color4_Normal"));

	// Loading Screen Texture2D 로드
	T_Color4_Normal = Cast<UTexture2D>(TextureRef.TryLoad());
}

void UW_HostGameMenu::OnMaxPlayerSwitchedHandler(const FString& SelectedOption)
{
	MaxAmount_SessionConnections = UKismetStringLibrary::Conv_StringToInt(SelectedOption);
}

void UW_HostGameMenu::OnConnectionTypeSwitchedHandler(const FString& SelectedOption)
{
	bConnectionType_LAN = (FString("LAN") == SelectedOption);
}

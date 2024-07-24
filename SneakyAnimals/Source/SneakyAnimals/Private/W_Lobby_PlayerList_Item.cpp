#include "W_Lobby_PlayerList_Item.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/SizeBox.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "GM_Base.h"
#include "GS_Lobby.h"

void UW_Lobby_PlayerList_Item::NativeConstruct()
{
	Super::NativeConstruct();

	// Button Event Bind
	if (Kick_Btn)
	{
		Kick_Btn->OnClicked.AddDynamic(this, &UW_Lobby_PlayerList_Item::KickPlayer);
	}

	Setup();
}

void UW_Lobby_PlayerList_Item::Setup()
{
	Set_Avatar();
	Set_Username();
	Set_HostIndicator();
	Set_ReadyStatus();
	Set_KickButton();
}

void UW_Lobby_PlayerList_Item::Set_Avatar()
{
	Player_Avatar_Image->SetBrushFromTexture(ConnectedPlayer.S_UserProfile.User_Avatar);
}

void UW_Lobby_PlayerList_Item::Set_Username()
{
	Player_Username_text->SetText(ConnectedPlayer.S_UserProfile.Username);
}

void UW_Lobby_PlayerList_Item::Set_HostIndicator()
{
	if (ConnectedPlayer.S_PlayerConnectionInfo.bIsHost)
	{
		HostIndicator->SetVisibility(ESlateVisibility::HitTestInvisible);
		return;
	} 
	else
	{
		HostIndicator->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
}

void UW_Lobby_PlayerList_Item::Set_ReadyStatus()
{
	if (ConnectedPlayer.S_PlayerConnectionInfo.bReadyStatus)
	{
		Player_ReadyStatus->SetColorAndOpacity(Ready_Color);
		Player_ReadyStatus->SetText(FText::FromString(TEXT("Ready")));
	} 
	else
	{
		Player_ReadyStatus->SetColorAndOpacity(NotReady_Color);
		Player_ReadyStatus->SetText(FText::FromString(TEXT("Not ready")));
	}
}

void UW_Lobby_PlayerList_Item::Set_KickButton()
{
	if (UKismetSystemLibrary::IsServer(GetWorld()))
	{
		// When we are the server (Host), then we simply disable our own Kick Button
		if (ConnectedPlayer.S_PlayerConnectionInfo.bIsHost)
		{
			Kick_Btn->SetIsEnabled(false);
		}
	} 
	else
	{
		// When we are a client, then we disable and hide the text inside all Kick buttons.
		Kick_Btn->SetIsEnabled(false);
		KickButton_Text->SetText(FText::FromString(TEXT("")));
	}
}

void UW_Lobby_PlayerList_Item::KickPlayer()
{
	Cast<AGM_Base>(UGameplayStatics::GetGameMode(GetWorld()))->KickPlayer(ConnectedPlayer.S_PlayerConnectionInfo.PlayerID);

	//FTimerHandle t;
	//GetWorld()->GetTimerManager().SetTimer(t, [&]() {
	//	Cast<AGS_Lobby>(UGameplayStatics::GetGameState(this))->Update_ConnectedPlayers_Array();
	//	}, 4.0f, false);

	// KYJ: Kick 하고 나서 Playerlist 가 제대로 설정된 후에 다시 업데이트 하도록
	Cast<AGS_Lobby>(UGameplayStatics::GetGameState(this))->Update_ConnectedPlayers_Array();
}

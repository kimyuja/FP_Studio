// Fill out your copyright notice in the Description page of Project Settings.


#include "W_ServerBrowserItem.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetTextLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h>
#include "W_Base_Button.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include "GI_SneakyAnimals.h"
#include "FL_General.h"

void UW_ServerBrowserItem::Setup(const FSessionInfo& info)
{
	SessionToJoin = info;
	
	LobbyName_Text->SetText(FText::FromString(info.hostName));
	FString count = FString::Printf(TEXT("(%d/%d)"), info.currentPlayerCount, info.maxPlayerCount);
	AmountOfPlayers_Text->SetText(FText::FromString(count));
	FString ping = FString::Printf(TEXT("%dms"), info.pingMs);
	Ping_Text->SetText(FText::FromString(ping));
}

void UW_ServerBrowserItem::NativeConstruct()
{
	Super::NativeConstruct();

	// join button delegate
	JoinSession_Btn->Button->OnClicked.AddDynamic(this, &UW_ServerBrowserItem::OnMyJoinSession_BtnClicked);

	// Loading Screen Texture2D 경로 설정
	FSoftObjectPath TextureRef(TEXT("/Game/KYJ/Assets/Widgets/General/T_Color4_Normal.T_Color4_Normal"));

	// Loading Screen Texture2D 로드
	T_Color4_Normal = Cast<UTexture2D>(TextureRef.TryLoad());

}

void UW_ServerBrowserItem::OnMyJoinSession_BtnClicked()
{
	// join session
	Cast<UGI_SneakyAnimals>(GetWorld()->GetGameInstance())->JoinMySession(SessionToJoin.index);

	// set session info
	UFL_General::Set_SessionInfo(GetWorld(), SessionToJoin.hostName, SessionToJoin.maxPlayerCount);

	// create loading screen
	FString loadingScreenFeedbackText = FString::Printf(TEXT("Joining %s\'s lobby"), *SessionToJoin.hostName);
	UFL_General::Create_LoadingScreen(GetWorld(), T_Color4_Normal, FText::FromString(loadingScreenFeedbackText));
}

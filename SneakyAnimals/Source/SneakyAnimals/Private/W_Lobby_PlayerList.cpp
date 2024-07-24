// Fill out your copyright notice in the Description page of Project Settings.


#include "W_Lobby_PlayerList.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/ScrollBox.h>
#include "W_Lobby_PlayerList_Item.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "FL_General.h"

UW_Lobby_PlayerList::UW_Lobby_PlayerList(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// WB_Lobby_PlayerList_Item ���� �������Ʈ Ŭ������ �ε��Ѵ�.
	static ConstructorHelpers::FClassFinder<UW_Lobby_PlayerList_Item> WidgetBPClass(TEXT("/Game/KYJ/Widgets/Lobby/WB_Lobby_PlayerList_Item.WB_Lobby_PlayerList_Item_C"));
	if (WidgetBPClass.Succeeded())
	{
		Lobby_PlayerList_Item_bp = WidgetBPClass.Class;
	}

}

void UW_Lobby_PlayerList::Update_Widget(TArray<FStructure_ConnectedPlayer> _Connected_Players)
{
	Connected_Players = _Connected_Players;
	Update_PlayerList();
	Update_PlayerCount();
}

void UW_Lobby_PlayerList::Update_PlayerList()
{
    if (!PlayerList)
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerList is not valid!"));
        return;
    }

    if (!Lobby_PlayerList_Item_bp)
    {
        UE_LOG(LogTemp, Error, TEXT("Lobby_PlayerList_Item_bp is not valid!"));
        return;
    }

    PlayerList->ClearChildren();

    // Populates Player List with players
    for (int32 Index = 0; Index < Connected_Players.Num(); ++Index)
    {
        FStructure_ConnectedPlayer& p = Connected_Players[Index];

        // Create a unique name for each widget instance
        // ���� �̸��� ����ũ�ؾ� ���� �� ������ �� �ִ�.
        // ���� �̸��� ���ƹ����� ������ ������ �����ȴ�.
        FName WidgetName = FName(*FString::Printf(TEXT("WB_Lobby_PlayerList_Item_%d"), Index));

        Lobby_PlayerList_Item_inst = CreateWidget<UW_Lobby_PlayerList_Item>(this, Lobby_PlayerList_Item_bp, WidgetName);
        if (!Lobby_PlayerList_Item_inst)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create Lobby_PlayerList_Item instance for player %s"), *p.S_UserProfile.Username.ToString());
            continue;
        }

        Lobby_PlayerList_Item_inst->ConnectedPlayer = p;

        Lobby_PlayerList_Item_inst->SetVisibility(ESlateVisibility::Visible);
        PlayerList->AddChild(Lobby_PlayerList_Item_inst);
    }

	return;
}

void UW_Lobby_PlayerList::Update_PlayerCount()
{
	FText InText = FText::Format(FText::FromString(TEXT("{0}/{1} players")), Connected_Players.Num(), UFL_General::Get_SessionInfo(GetWorld()).maxPlayerCount);

	PlayerCount_Text->SetText(InText);
	
	return;
}

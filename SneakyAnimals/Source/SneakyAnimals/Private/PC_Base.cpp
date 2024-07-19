// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Base.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h>
#include "FL_General.h"

void APC_Base::ClientRPC_Create_MapTravel_LoadingScreen_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("APC_Base ClientRPC_Create_MapTravel_LoadingScreen (All PCs)"));
	CleanUp_UI();
	// ���� ������ �� �� �̹��� ���� �ֱ� KYJ
	UFL_General::Create_LoadingScreen(GetWorld(), T_Color4_Normal, FText::FromString(TEXT("Traveling to  GameMaps")));
}

void APC_Base::BeginPlay()
{
	Super::BeginPlay();
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);

	// Loading Screen Texture2D ��� ����
	FSoftObjectPath TextureRef(TEXT("/Game/KYJ/Assets/Widgets/General/T_Color4_Normal.T_Color4_Normal"));

	// Loading Screen Texture2D �ε�
	T_Color4_Normal = Cast<UTexture2D>(TextureRef.TryLoad());
}

void APC_Base::CleanUp_UI()
{
	UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
}

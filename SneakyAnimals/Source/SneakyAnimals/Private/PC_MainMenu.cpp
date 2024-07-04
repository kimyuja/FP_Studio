// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_MainMenu.h"
#include "W_MainMenu.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>

APC_MainMenu::APC_MainMenu()
{
	// WB_MainMenu ���� �������Ʈ Ŭ������ �ε��Ѵ�.
	static ConstructorHelpers::FClassFinder<UW_MainMenu> WidgetBPClass(TEXT("/Game/KYJ/Widgets/MainMenu/WB_MainMenu.WB_MainMenu_C"));
	if (WidgetBPClass.Succeeded())
	{
		MainMenu_bp = WidgetBPClass.Class;
	}
}

void APC_MainMenu::BeginPlay()
{
	Super::BeginPlay();

	// ���� UI�� ȭ�鿡 ����Ѵ�.
	if (MainMenu_bp != nullptr)
	{
		if (IsLocalPlayerController())
		{
			MainMenu_inst = CreateWidget<UW_MainMenu>(this, MainMenu_bp, FName("WB_MainMenu"));

			if (MainMenu_inst)
			{
				MainMenu_inst->AddToViewport(0);
				UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this, MainMenu_inst, EMouseLockMode::DoNotLock);
			}
		}
	}

	return;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDieLogWidget.h"
#include "PS_Gameplay.h"
#include "Gimmick.h"
#include "Components/TextBlock.h"
#include "Internationalization/Text.h"
#include "TimerManager.h"



void UPlayerDieLogWidget::ShowText(const FText& NewText)
{
	if (DieLogText)
	{
		DieLogText->SetText(NewText);
	}

	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, [this]()
		{
			RemoveFromParent();
		}, 1.5f, false);


}

void UPlayerDieLogWidget::GetLogSet(FText ActivePlayer, AGimmick* gimmickActor, int32 ActiveType)
{	
	playerName = ActivePlayer;

	txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} died from electrocution!"), playerName);
	ShowText(txt);
}

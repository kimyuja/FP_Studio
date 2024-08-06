// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDieLogWidget.h"
#include "PS_Gameplay.h"
#include "Gimmick.h"
#include "Components/TextBlock.h"
#include "Internationalization/Text.h"
#include "TimerManager.h"
#include "WH_BookshelfGimmick.h"
#include "WH_BroomstickGimmick.h"
#include "WH_PotionGimmick.h"
#include "WH_WitchCauldronGimmick.h"
#include "SM_ComputerGimmick.h"
#include "SM_PeriscopeGimmick.h"
#include "SM_PressButtonGimmick.h"
#include "SM_WhistleGimmick.h"
#include "SP_BottleGimmick.h"
#include "SP_CartGimmick.h"
#include "SP_CleanerGimmick.h"
#include "SP_ShowcaseGimmick.h"
#include "BS_GoldBarGimmick.h"
#include "BS_HandleGimmick.h"
#include "BS_LaserGimmick.h"
#include "BS_SwitchGimmick.h"



void UPlayerDieLogWidget::ShowText(const FText& NewText)
{
	if (DieLogText)
	{
		DieLogText->SetText(NewText);
	}

	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, [this]()
		{
			RemoveFromParent();
		}, 3.0f, false);


}

void UPlayerDieLogWidget::GetLogSet(FText _ActivePlayer, AGimmick* _GimmickActor, int32 _ActiveType)
{	
	playerName = _ActivePlayer;

	// 마녀의 집
	if (_GimmickActor->IsA(AWH_BookshelfGimmick::StaticClass()))
	{
		switch (_ActiveType)
		{
		case 0:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} was crushed to death by a bookshelf with other players!"), playerName);
			break;
		case 1:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} died by a book that flew in!"), playerName);
			break;
		default:
			break;
		}
	}
	else if (_GimmickActor->IsA(AWH_BroomstickGimmick::StaticClass()))
	{
		switch (_ActiveType)
		{
		case 0:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} was beaten to death by a broom!"), playerName);
			break;
		case 1:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} was flying on a broom and fell to death!"), playerName);
			break;
		default:
			break;
		}
	}
	else if (_GimmickActor->IsA(AWH_PotionGimmick::StaticClass()))
	{
		switch (_ActiveType)
		{
		case 0:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} took potions and got smaller and then disappeared!"), playerName);
			break;
		case 1:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "Because of {0}, the potion burst and blew away the players around there"), playerName);
			break;
		default:
			break;
		}

	}
	else if (_GimmickActor->IsA(AWH_WitchCauldronGimmick::StaticClass()))
	{
		switch (_ActiveType)
		{
		case 0:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} was suffocated by the poison smoke!"), playerName);
			break;
		case 1:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "Witch appeared and killed {0}!"), playerName);
			break;
		default:
			break;
		}
	}
	// 잠수함
	else if (_GimmickActor->IsA(ASM_ComputerGimmick::StaticClass()))
	{
		switch (_ActiveType)
		{
		case 0:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} was electrocuted!"), playerName);
			break;
		case 1:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} activated the computer's explosive device and died!"), playerName);
			break;
		default:
			break;
		}
	}
	else if (_GimmickActor->IsA(ASM_PeriscopeGimmick::StaticClass()))
	{
		switch (_ActiveType)
		{
		case 0:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} was killed by a rotating periscope"), playerName);
			break;
		case 1:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} was killed by falling periscope!"), playerName);
			break;
		default:
			break;
		}
	}
	else if (_GimmickActor->IsA(ASM_PressButtonGimmick::StaticClass()))
	{
		switch (_ActiveType)
		{
		case 0:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} has opened the door to a submarine in the water!"), playerName);
			break;
		case 1:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "During the blackout, {0} died!"), playerName);
			break;
		default:
			break;
		}
	}
	else if (_GimmickActor->IsA(ASM_WhistleGimmick::StaticClass()))
	{
		switch (_ActiveType)
		{
		case 0:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} died of a heart attack after seeing the shark!"), playerName);
			break;
		case 1:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} choked to death by swallowing a whistle!"), playerName);
			break;
		default:
			break;
		}
	}
	// 슈퍼마켓
	else if (_GimmickActor->IsA(ASP_BottleGimmick::StaticClass()))
	{
		switch (_ActiveType)
		{
		case 0:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} stepped on a drink bottle and fell!"), playerName);
			break;
		case 1:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} fell while kicking a drink bottle!"), playerName);
			break;
		default:
			break;
		}
	}
	else if (_GimmickActor->IsA(ASP_CartGimmick::StaticClass()))
	{
		switch (_ActiveType)
		{
		case 0:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} fell and died because the cart handle fell off!"), playerName);
			break;
		case 1:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} died after being hit by a cart!"), playerName);
			break;
		default:
			break;
		}
	}
	else if (_GimmickActor->IsA(ASP_CleanerGimmick::StaticClass()))
	{
		switch (_ActiveType)
		{
		case 0:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} died after drinking lox!"), playerName);
			break;
		case 1:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} stuck his foot in a cleaning tool and died!"), playerName);
			break;
		default:
			break;
		}
	}
	else if (_GimmickActor->IsA(ASP_ShowcaseGimmick::StaticClass()))
	{
		switch (_ActiveType)
		{
		case 0:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} knocked over the show case!"), playerName);
			break;
		case 1:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} died from eating weird food!"), playerName);
			break;
		default:
			break;
		}
	}
	// 금고
	else if (_GimmickActor->IsA(ABS_GoldBarGimmick::StaticClass()))
	{
		switch (_ActiveType)
		{
		case 0:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} was caught by the police for touching a gold bar!"), playerName);
			break;
		case 1:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} died after banging his head on a gold bar!"), playerName);
			break;
		default:
			break;
		}
	}
	else if (_GimmickActor->IsA(ABS_HandleGimmick::StaticClass()))
	{
		switch (_ActiveType)
		{
		case 0:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} was electrocuted by static electricity!"), playerName);
			break;
		case 1:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} touched the security and they're all caught by the police!"), playerName);
			break;
		default:
			break;
		}
	}
	else if (_GimmickActor->IsA(ABS_LaserGimmick::StaticClass()))
	{
		switch (_ActiveType)
		{
		case 0:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} called the sheriff and they were all arrested!"), playerName);
			break;
		case 1:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} pressed the switch and flew away!"), playerName);
			break;
		default:
			break;
		}
	}
	else if (_GimmickActor->IsA(ABS_SwitchGimmick::StaticClass()))
	{
		switch (_ActiveType)
		{
		case 0:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} knocked over the show case!"), playerName);
			break;
		case 1:
			txt = FText::Format(NSLOCTEXT("YourNamespace", "DeathMessage", "{0} died from eating weird food!"), playerName);
			break;
		default:
			break;
		}
	}

	
	ShowText(txt);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemManager.h"

UItemManager* UItemManager::Instance = nullptr;

UItemManager* UItemManager::Get()
{
    if (!Instance)
    {
        Instance = NewObject<UItemManager>();
        Instance->AddToRoot(); // Ensure it is not garbage collected
    }
    return Instance;
}

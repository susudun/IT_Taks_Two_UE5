// Fill out your copyright notice in the Description page of Project Settings.


#include "Frameworks/GameInstances/ITTGameInstance.h"

#include "Characters/BaseCharacter.h"
#include "Controllers/BasePlayerController.h"
#include "SystemActor/CheckPoint.h"

#include "SystemActor/CheckPointData.h"

DEFINE_LOG_CATEGORY(LogGIS)

void UITTGameInstance::Init()
{
	Super::Init();
	
	InitCheckPointIndex = 0;
	CurrentCheckPointIndex = 0;
	CheckPointMap.Empty();


}

void UITTGameInstance::RegisterCheckPoint(ACheckPoint* SpawnedCheckPoint)
{
	SpawnedCheckPoint->OnCheckPointActivated.AddDynamic(this, &UITTGameInstance::ActivateCheckPoint);

	FCheckPointData NewCheckPointData(SpawnedCheckPoint, InitCheckPointIndex);
	CheckPointMap.Add(InitCheckPointIndex, NewCheckPointData);
	UE_LOG(LogGIS, Log, TEXT("Register CheckPoint : %s, Index : %d"), *SpawnedCheckPoint->GetName(), InitCheckPointIndex)
	InitCheckPointIndex += 1;
}

void UITTGameInstance::ActivateCheckPoint(uint8 CheckPointIndex)
{
	for (TPair<uint8, FCheckPointData>& CheckPointData : CheckPointMap)
	{
		if (CheckPointData.Key == CheckPointIndex)
		{
			if (CheckPointData.Value.IsActivatedCheckPoint())
			{
				continue;
			}
			CheckPointData.Value.ActivateCheckPoint();
			CurrentCheckPointIndex = CheckPointIndex;
			//UE_LOG(LogGIS, Log, TEXT("Activate CheckPoint : %d"), CheckPointData.Key)
		}
		else
		{
			CheckPointData.Value.DeactivateCheckPoint();
			//UE_LOG(LogGIS, Log, TEXT("Deactivate CheckPoint : %d"), CheckPointData.Key)
		}
	}
}

bool UITTGameInstance::GetCheckPointIsActivateByID(uint8 InID)
{
	if (CheckPointMap.Contains(InID))
	{
		return CheckPointMap[InID].IsActivatedCheckPoint();
	}
	return false;
}

void UITTGameInstance::RespawnPlayerCharacter(APlayerController* PC)
{
	
	FCheckPointData* CheckPointData = CheckPointMap.Find(CurrentCheckPointIndex);
	if (not CheckPointData->IsActivatedCheckPoint())
	{
		UE_LOG(LogGIS, Log, TEXT("No CheckPoint is Activated."))
		return;
	}
	CheckPointData->TryAwakeCheckPoint(GetWorld());
	
	AActor* player = PC->GetPawn();
	FVector SpawnLocation = CheckPointData->GetCheckPointRef()->GetActorLocation();
	FRotator SpawnRotation = CheckPointData->GetCheckPointRef()->GetActorRotation();
	SpawnLocation.Z += 150.0f;
	
	player->SetActorLocation(SpawnLocation);
	player->SetActorRotation(SpawnRotation);
}

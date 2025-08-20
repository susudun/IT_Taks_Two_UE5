// Fill out your copyright notice in the Description page of Project Settings.


#include "Frameworks/GameStates/BaseGameState.h"

#include "Controllers/BasePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogGS)
void ABaseGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseGameState,AllPlayerState);
	DOREPLIFETIME(ABaseGameState,AllPlayerCharacter);
	DOREPLIFETIME(ABaseGameState,Client_0);
	DOREPLIFETIME(ABaseGameState,Client_1);
	DOREPLIFETIME(ABaseGameState,CurrentBGM);
}

void ABaseGameState::OnRep_CurrentBGM()
{
	ABasePlayerController* PC = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PC)
	{
		PC->HandleBGMChange(CurrentBGM);
	}
}
#pragma region Client_Number_Check
void ABaseGameState::OnRep_Client_0()
{
	if (Client_0)
	{
		UE_LOG(LogGS, Warning, TEXT("Client_0: Cody Choice"));
	}
	else
	{
		UE_LOG(LogGS, Warning, TEXT("Client_0: May Choice"));
	}
	OnClient_0CharacterChoice.Broadcast(Client_0);
}

void ABaseGameState::OnRep_Client_1()
{
	if (Client_1)
	{
		UE_LOG(LogGS, Warning, TEXT("Client_1: Cody Choice"));
	}
	else
	{
		UE_LOG(LogGS, Warning, TEXT("Client_1: May Choice"));
	}
	OnClient_1CharacterChoice.Broadcast(Client_1);
}
#pragma endregion 

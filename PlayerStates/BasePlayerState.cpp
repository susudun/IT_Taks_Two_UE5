// Fill out your copyright notice in the Description page of Project Settings.


#include "Frameworks/PlayerStates/BasePlayerState.h"

#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "Frameworks/PlayerStates/BasePlayerState.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogPS);
ABasePlayerState::ABasePlayerState()
{
	// PlayerState는 항상 복제되어야 합니다.
	bReplicates = true;
	
	BaseAbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>(TEXT("BaseAbilitySystemComponent"));
	BaseAbilitySystemComponent->SetIsReplicated(true);
	BaseAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	BaseAttributeSet = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("BaseAttributeSet"));
}

UAbilitySystemComponent* ABasePlayerState::GetAbilitySystemComponent() const
{
	return BaseAbilitySystemComponent;
}

void ABasePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABasePlayerState, BaseAbilitySystemComponent);
	DOREPLIFETIME(ABasePlayerState, CharaterNumber);
}
#pragma region Server_Traveling_SavePlayerState
void ABasePlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
	if (ABasePlayerState* OldState = Cast<ABasePlayerState>(PlayerState))
	{
		 OldState->CharaterNumber=CharaterNumber;
	}
	else
	{
		UE_LOG(LogPS, Error, TEXT("OldState is nullptr or cast failed!"));
	}
}
#pragma endregion

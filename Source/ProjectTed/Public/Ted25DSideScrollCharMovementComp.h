// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Ted25DSideScrollCharMovementComp.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTED_API UTed25DSideScrollCharMovementComp : public UCharacterMovementComponent
{
	GENERATED_BODY()
	//主要就是重写这个函数
	virtual FRotator ComputeOrientToMovementRotation(const FRotator& CurrentRotation, float DeltaTime, FRotator& DeltaRotation) const override;
};

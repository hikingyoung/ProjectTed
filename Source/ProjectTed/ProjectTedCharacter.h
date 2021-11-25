// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Ted25DSideScrollCharMovementComp.h"
#include "ProjectTedCharacter.generated.h"

UCLASS(config=Game)
class AProjectTedCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

protected:

	/** Called for side to side input */
	void MoveRight(float Val);

	//Ted
	void MoveUp(float Val);
	
	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

private:
	UTed25DSideScrollCharMovementComp* Ted25DSideScrollCharMovementComp;
public:
	UFUNCTION(BlueprintCallable, Category ="Ted25DSideScroller")
	//FORCEINLINE强制内联。 尾置const表示此方法不可更改调用者本身。
	FORCEINLINE class UTed25DSideScrollCharMovementComp* GetTed25DSideScrollCharMovementComp() const { return Ted25DSideScrollCharMovementComp; }
	public:
	AProjectTedCharacter(const FObjectInitializer* ObjectInitializer);
	
	virtual void PostInitializeComponents() override;
	
	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};




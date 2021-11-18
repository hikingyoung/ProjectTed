// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectTedCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h" //原先无需



//////////////////////////////////////////////////////////////////////////
// AProjectTedCharacter

AProjectTedCharacter::AProjectTedCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f,180.f,0.f));

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate. Overried Downbelow by Ted.
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;
	
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	//Ted： Set these for 2.5D side scroller.
	GetCharacterMovement()->RotationRate = FRotator(0.f, -1.f, 0.f);
	GetCharacterMovement()->MaxAcceleration = 100000;
	GetCharacterMovement()->BrakingDecelerationWalking = 100000;
	GetCharacterMovement()->bUseControllerDesiredRotation = false; //不受控制器，如鼠标，影响朝向。这样按上也不会面向上了。
	GetCharacterMovement()->bOrientRotationToMovement = false; //不与加速方向对齐，这样往上移动时就不会向上了。
}

//////////////////////////////////////////////////////////////////////////
// Input

void AProjectTedCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &AProjectTedCharacter::MoveRight);
	//Ted:Add new.
	PlayerInputComponent->BindAxis("MoveForward", this, &AProjectTedCharacter::MoveUp);
	
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AProjectTedCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AProjectTedCharacter::TouchStopped);
}

void AProjectTedCharacter::MoveRight(float Value)
{
	// add movement in that direction。 Ted回头这里要处理右上一起按的数值问题
	AddMovementInput(FVector(0.f,-1.f,0.f), Value);

	//Ted人物的朝向不受控制器也不受加速方向影响（按向上不转向，也可以做后跳，侧步等技能）。整个人物唯一会改变方向的可能就在此处。
	//同时也带来新的问题，就是如果怪物也以此为蓝本，因为怪物是不存在按键的，它们移动的时候要如何处理？？
	this->SetActorRotation(FRotator(0.f,-90*Value/abs(Value),0.f)); //TODO：这里有分母为0陷阱。暂时还未报错。
}

//Ted
void AProjectTedCharacter::MoveUp(float Value)
{	
	//add movement in that direction。 Ted 回头这里要处理右上一起按的数值问题，根号2大于1.
	AddMovementInput(FVector(-1.f, 0.f, 0.f), Value);
}




void AProjectTedCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void AProjectTedCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

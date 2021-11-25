// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectTedCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h" //原先无需
#include "Ted25DSideScrollCharMovementComp.h"


//////////////////////////////////////////////////////////////////////////
// AProjectTedCharacter

AProjectTedCharacter::AProjectTedCharacter(const FObjectInitializer& ObjectInitializer):
Super(ObjectInitializer.SetDefaultSubobjectClass<UTed25DSideScrollCharMovementComp>(ACharacter::CharacterMovementComponentName))
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
	//ArrowComponent从ACharacter中继承而来，它决定人物的真实朝向，不管人物动画怎样，只要arrow没变，人物的朝向就不会变。
	//这一条决定了不管人物自身怎么转，摄像机都是不会转的。除非单独调整摄像机。
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	//Ted: 从默认500改成800，后续再调
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	//Ted: 这个180度决定摄像机照人物哪一边。在场景中调整人物的转向，摄像机因为使用的是绝对旋转，是不受影响的。
	//这个-15度是摄像机的俯仰角，改成-15时观察体验更好，这个后续可以再调。
	CameraBoom->SetRelativeRotation(FRotator(-15.f,180.f,0.f));
	
	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera
	
	// Configure character movement
	//GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate. Overried Downbelow by Ted.
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	//GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->JumpZVelocity=2000.f;
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
	GetCharacterMovement()->bOrientRotationToMovement = true; //2021.11.18不与加速方向对齐，这样往上移动时就不会向上了。 //2021.11.19 因为更改了底层计算加速方向的方式，详见Ted25DSideScrollCharMoveComp.cpp, 所以改为true.
	//Ted：set this true then when you go near a wall, your sight will no longer be blocked by the wall,
	//just become closer to root component(usually it's a character body)
	GetCameraBoom()->bDoCollisionTest = true;
}

void AProjectTedCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//ted
	Ted25DSideScrollCharMovementComp = Cast<UTed25DSideScrollCharMovementComp>(Super::GetCharacterMovement());
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
	//this->SetActorRotation(FRotator(0.f,-90*Value/abs(Value),0.f));//因为底层加速方向的算法改了，所以暂时注释此行来测试 //TODO：这里有分母为0陷阱。暂时还未报错。
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

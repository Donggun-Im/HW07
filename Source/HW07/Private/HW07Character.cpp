// Fill out your copyright notice in the Description page of Project Settings.

#include "EnhancedInputSubsystems.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "HW07Character.h"

#include "InterchangeResult.h"
#include "KismetTraceUtils.h"

// Sets default values
AHW07Character::AHW07Character()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	SetRootComponent(Capsule);
	Capsule->SetSimulatePhysics(false);
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->bUsePawnControlRotation = true;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	SkeletalMesh->SetSimulatePhysics(false);
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
}

// Called when the game starts or when spawned
void AHW07Character::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputContext,0);
		}
		PC->SetViewTarget(this);
	}
	CurrentRotation = Controller->GetControlRotation();
	CurrentLocation = GetActorLocation();
}

void AHW07Character::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddGravity(DeltaTime);
	
}


// Called to bind functionality to input
void AHW07Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetupPlayerInputComponent"));
		EnhancedInputComponent->BindAction(IAMove, ETriggerEvent::Triggered, this, &AHW07Character::Move);
		EnhancedInputComponent->BindAction(IALook, ETriggerEvent::Triggered, this, &AHW07Character::Look);
		EnhancedInputComponent->BindAction(IAUpDown, ETriggerEvent::Triggered, this, &AHW07Character::UpDown);
		EnhancedInputComponent->BindAction(IARoll, ETriggerEvent::Triggered, this, &AHW07Character::Roll);
	}
}

void AHW07Character::Move(const FInputActionValue& Value)
{
	FVector2D Move = Value.Get<FVector2D>();
	AddActorLocalOffset(FVector(Move.X, 0, Move.Y) * MoveSpeed);
	UE_LOG(LogTemp, Warning, TEXT("Move"));
}

void AHW07Character::Look(const FInputActionValue& Value)
{
	FVector2D MouseMove = Value.Get<FVector2D>();
	CurrentRotation +=FRotator(MouseMove.Y,MouseMove.X, 0 )*LookSpeed;
	SetActorRotation(CurrentRotation);
	Controller->SetControlRotation(CurrentRotation);
	UE_LOG(LogTemp, Warning, TEXT("Look"));
}

void AHW07Character::UpDown(const FInputActionValue& Value)
{
	float UpDownValue = Value.Get<float>();
	if (UpDownValue > 0)
	{
		UpDownSpeed = 0;
	}
	AddActorWorldOffset(FVector(0, 0, UpDownValue * MoveSpeed));
}

void AHW07Character::Roll(const FInputActionValue& Value)
{
	float RollValue = Value.Get<float>();
	CurrentRotation.Roll += RollValue*RollSpeed;
	SetActorRotation(CurrentRotation);
	Controller->SetControlRotation(CurrentRotation);
	UE_LOG(LogTemp, Warning, TEXT("Roll"));
}



void AHW07Character::AddGravity(const float DeltaTime)
{
	if (!IsGrounded())
	{
		UpDownSpeed += DeltaTime * UpDownGravityAcceleration;
		UE_LOG(LogTemp, Warning, TEXT("Flying"));
	}
	else
	{
		if (UpDownSpeed < 0)
		{
			UpDownSpeed = 0;
		}
		UE_LOG(LogTemp, Warning, TEXT("Grounded"));
	}
	AddActorWorldOffset(FVector(0, 0, DeltaTime * UpDownSpeed));
}

bool AHW07Character::IsGrounded() const
{
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	//ActorLineTraceSingle(Hit, GetActorLocation(), GetActorLocation() + FVector(0, 0, -1000), ECollisionChannel::ECC_WorldStatic, Params );
	DrawDebugLineTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(0, 0, -100), EDrawDebugTrace::ForOneFrame, true, Hit,FColor::Red,FColor::Red, 1000);
	if (UWorld* World = GetWorld())
	{
		World->LineTraceSingleByChannel(Hit, GetActorLocation(), GetActorLocation() + FVector(0, 0, -100), ECollisionChannel::ECC_WorldStatic, Params);
	}
	//ActorLineTraceSingle(Hit, GetActorLocation(), GetActorLocation() + FVector(0, 0, -1000), ECollisionChannel::ECC_WorldStatic, Params);
	if (Hit.bBlockingHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *Hit.GetActor()->GetName());
	}
	return Hit.bBlockingHit;

}






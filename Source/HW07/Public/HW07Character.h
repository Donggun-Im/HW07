// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Pawn.h"
#include "HW07Character.generated.h"

class UInputMappingContext;
class UInputAction;
class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class UInputComponent;

UCLASS()
class HW07_API AHW07Character : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHW07Character();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "Character")
	UCapsuleComponent* Capsule;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "Character")
	USpringArmComponent* SpringArm;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "Character")
	UCameraComponent* Camera;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "Character")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "Input")
	UInputMappingContext* InputContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "Input")
	UInputAction* IAMove;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "Input")
	UInputAction* IALook;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "Input")
	UInputAction* IAUpDown;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "Input")
	UInputAction* IARoll;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "Character")
	float MoveSpeed = 100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "Character")
	float LookSpeed = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Character")
	float UpDownSpeed =0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Character")
	float UpDownGravityAcceleration =-98.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Character")
	float RollSpeed = 5.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Character")
	FRotator CurrentRotation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Character")
	FVector CurrentLocation;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void UpDown(const FInputActionValue& Value);
	UFUNCTION()
	void Roll(const FInputActionValue& Value);
	
	UFUNCTION()
	bool IsGrounded() const;
	UFUNCTION()
	void AddGravity(float DeltaTime);
};

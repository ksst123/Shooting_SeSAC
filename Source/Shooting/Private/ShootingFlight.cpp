// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingFlight.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Bullet.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Sets default values
AShootingFlight::AShootingFlight()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 충돌 박스 컴포넌트 생성
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	// 생성된 충돌 박스 컴포넌트를 루트 컴포넌트로 설정
	RootComponent = BoxComponent; // == SetRootComponent(BoxComponent);
	// 박스 콜리전 크기를 (50, 50, 50) 으로 설정
	BoxComponent->SetBoxExtent(FVector(50));


	// 메시 컴포넌트 생성
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	// 생성된 메시 컴포넌트를 루트 컴포넌트의 하위 개체로 등록
	MeshComponent->SetupAttachment(RootComponent);
	// Shape_Cube 파일을 찾아서 변수로 생성
	ConstructorHelpers::FObjectFinder<UStaticMesh> CubeStaticMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	// 만일 큐브 파일을 로드하는데 성공했다면
	if (CubeStaticMesh.Succeeded())
	{
		// 로드한 파일을 메시 컴포넌트의 Static Mesh 항목 할당
		MeshComponent->SetStaticMesh(CubeStaticMesh.Object);
	}
	// MeshComponent->SetWorldLocation(FVector(0, 0, -50));

}

// Called when the game starts or when spawned
void AShootingFlight::BeginPlay()
{
	Super::BeginPlay();
	
	// 플레이어 컨트롤러를 캐스팅
	// APlayerController* PlayerController = Cast<APlayerController>(Controller);
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (SubSystem != nullptr)
		{
			SubSystem->AddMappingContext(IMC_MyMapping, 0);
		}
	}
}

// Called every frame
void AShootingFlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// direction 벡터 정규화
	// 사용자가 입력한 방향대로 이동
	// P = P0 + vt == SetActorLocation = GetActorLocation + (direction(방향) * speed(크기)) * DeltaTime
	// 방향*크기 = 속도, Scalar = 속도, Delta는 경과 중인 흐름 의미
	direction.Normalize(); // 길이가 1인 벡터 = 단위 벡터
	FVector dir = GetActorLocation() + (direction * MoveSpeed) * DeltaTime;
	SetActorLocation(dir);

}

// Called to bind functionality to input
void AShootingFlight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 기존의 UInputComponent* 변수를 UEnhancedInputComponent 로 변환
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// 액션 함수에 연결
	EnhancedInputComponent->BindAction(IA_MoveHorizontal, ETriggerEvent::Triggered, this, &AShootingFlight::MoveHorizontal);
	EnhancedInputComponent->BindAction(IA_MoveHorizontal, ETriggerEvent::Completed, this, &AShootingFlight::MoveHorizontal);

	EnhancedInputComponent->BindAction(IA_MoveVertical, ETriggerEvent::Triggered, this, &AShootingFlight::MoveVertical);
	EnhancedInputComponent->BindAction(IA_MoveVertical, ETriggerEvent::Completed, this, &AShootingFlight::MoveVertical);

	EnhancedInputComponent->BindAction(IA_FireBullet, ETriggerEvent::Triggered, this, &AShootingFlight::FireBullet);

	//// Horizontal Axis 입력에 함수를 연결
	//PlayerInputComponent->BindAxis("MoveHorizontal", this, &AShootingFlight::MoveHorizontal);
	//// Vertical Axis 입력에 함수를 연결
	//PlayerInputComponent->BindAxis("MoveVertical", this, &AShootingFlight::MoveVertical);

	//// Fire Action 입력에 함수를 연결
	//PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AShootingFlight::FireBullet);
}

//// 좌우 입력이 있을 때 실행될 함수
//void AShootingFlight::MoveHorizontal(float value) {
//	
//	// 입력 value 출력
//	h = value;
//	// UE_LOG(LogTemp, Warning, TEXT("h : %.4f"), h);
//
//	direction.Y = h;
//}
//
//// 상하 입력이 있을 때 실행될 함수
//void AShootingFlight::MoveVertical(float value) {
//	
//	// 입력 value 출력
//	v = value;
//	// UE_LOG(LogTemp, Warning, TEXT("h : %.4f"), v);
//
//	direction.Z = v;
//}
//
//// 마우스 왼쪽 버튼을 눌렀을 때 실행될 함수
//void AShootingFlight::FireBullet() {
//	
//	// Bullet 생성 위치를 담을 변수 생성
//	FVector SpawnPosition = GetActorLocation() + GetActorUpVector() * 90.0f;
//	// Bullet 생성 시 회전값을 담을 변수 생성
//	// 에디터와는 다르게 코드에서는 FRotator(Pitch, Yaw, Roll) 순서
//	FRotator SpawnRotation = FRotator(90.0f, 0, 0);
//	FActorSpawnParameters SpawnParam;
//	// 오브젝트 생성 시 생성 위치에 충돌 가능한 다른 오브젝트가 있을 경우에 어떻게 할 것인지 설정하는 옵션
//	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//
//	// Bullet을 생성
//	// Bullet Blueprint 변수
//	GetWorld()->SpawnActor<ABullet>(BulletFactory, SpawnPosition, SpawnRotation, SpawnParam);
//}

void AShootingFlight::MoveHorizontal(const FInputActionValue& value) {
	h = value.Get<float>();
	// UE_LOG(LogTemp, Warning, TEXT("h : %.4f"), h);
	direction.Y = h;
}

void AShootingFlight::MoveVertical(const FInputActionValue& value) {
	v = value.Get<float>();
	// UE_LOG(LogTemp, Warning, TEXT("v : %.4f"), v);
	direction.Z = v;
}

void AShootingFlight::FireBullet() {
		
	// Bullet 생성 위치를 담을 변수 생성
	FVector SpawnPosition = GetActorLocation() + GetActorUpVector() * 90.0f;
	// Bullet 생성 시 회전값을 담을 변수 생성
	// 에디터와는 다르게 코드에서는 FRotator(Pitch, Yaw, Roll) 순서
	FRotator SpawnRotation = FRotator(90.0f, 0, 0);
	FActorSpawnParameters SpawnParam;
	// 오브젝트 생성 시 생성 위치에 충돌 가능한 다른 오브젝트가 있을 경우에 어떻게 할 것인지 설정하는 옵션
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Bullet을 생성
	// Bullet Blueprint 변수
	GetWorld()->SpawnActor<ABullet>(BulletFactory, SpawnPosition, SpawnRotation, SpawnParam);
}

#include "pch.h"
#include "Node.h"

#include "Animation.h"
#include "Mesh.h"
#include "Model.h"

struct NodeTransformConstantBuffer
{
	Matrix mNodeWorld;
};

Node::Node(std::string name, Model* pOwner, Matrix localTransform, const std::vector<Mesh*>& meshes)
	: m_name(name)
	, m_pOwner(pOwner)
	, m_localTransform(localTransform)
	, m_meshes(meshes)
{
	this->SetupNode();
	this->UpdateTransform();
}

Node::~Node()
{
	for (auto& node : m_pChildren)
	{
		SAFE_DELETE(node);
	}
	m_pChildren.clear();

	for (auto& mesh : m_meshes)
	{
		SAFE_DELETE(mesh);
	}
	m_meshes.clear();
}

void Node::AddChildNode(Node* child)
{
	child->m_pParentNode = this;
	child->UpdateTransform();
	m_pChildren.push_back(child);
}

void Node::Update(float deltaTime)
{
	if (m_pNodeAnimation)
	{
		m_animationSpeed = m_pOwner->GetAnimationSpeed();
		m_elapsedTime += (deltaTime * m_animationSpeed);

		int nextIdx = (m_curIdx + 1) % (m_pNodeAnimation->Frames.size());
		if (m_elapsedTime > m_pNodeAnimation->Frames[nextIdx].FrameTime)
		{
			m_curIdx = nextIdx;
			if (m_curIdx == 0)
			{
				m_elapsedTime = 0.f;
			}
		}

		nextIdx = (m_curIdx + 1) % (m_pNodeAnimation->Frames.size());
		Vector3 prePosition = m_pNodeAnimation->Frames[m_curIdx].FramePosition;
		Vector3 preScale = m_pNodeAnimation->Frames[m_curIdx].FrameScale;
		Quaternion preQuaternion = m_pNodeAnimation->Frames[m_curIdx].FrameQuaternion;
		Vector3 nextPosition = m_pNodeAnimation->Frames[nextIdx].FramePosition;
		Vector3 nextScale = m_pNodeAnimation->Frames[nextIdx].FrameScale;
		Quaternion nextQuaternion = m_pNodeAnimation->Frames[nextIdx].FrameQuaternion;

		float t = (m_elapsedTime - m_pNodeAnimation->Frames[m_curIdx].FrameTime) /
			((m_pNodeAnimation->Frames[nextIdx].FrameTime) - (m_pNodeAnimation->Frames[m_curIdx].FrameTime));

		Vector3 position = Vector3::Lerp(prePosition, nextPosition, t);
		Vector3 scale = Vector3::Lerp(preScale, nextScale, t);
		Quaternion quaternion = Quaternion::Slerp(preQuaternion, nextQuaternion, t);

		m_localTransform = Matrix::CreateScale(scale) * Matrix::CreateFromQuaternion(quaternion) * Matrix::CreateTranslation(position);
	}


	UpdateTransform();

	for (const auto& child : m_pChildren)
	{
		child->Update(deltaTime);
	}
}

void Node::Render()
{
	CommonApp::m_pInstance->GetDevcon()->VSSetConstantBuffers(3, 1, m_pNodeTransformConstantBuffer.GetAddressOf());

	NodeTransformConstantBuffer NTCB;
	NTCB.mNodeWorld = XMMatrixTranspose(m_worldTransform);
	CommonApp::m_pInstance->GetDevcon()->UpdateSubresource(m_pNodeTransformConstantBuffer.Get(), 0, nullptr, &NTCB, 0, 0);

	for (auto& mesh : m_meshes)
	{
		mesh->Render();
	}

	for (const auto& child : m_pChildren)
	{
		child->Render();
	}
}

void Node::SetupNode()
{
	D3D11_BUFFER_DESC NTCBDesc = {};
	ZeroMemory(&NTCBDesc, sizeof(NTCBDesc));
	NTCBDesc.Usage = D3D11_USAGE_DEFAULT;
	NTCBDesc.ByteWidth = sizeof(NodeTransformConstantBuffer);
	NTCBDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	NTCBDesc.CPUAccessFlags = 0;
	HR_T(CommonApp::m_pInstance->GetDevice()->CreateBuffer(&NTCBDesc, nullptr, &m_pNodeTransformConstantBuffer));
}

void Node::UpdateTransform()
{
	if (m_pParentNode)
	{
		m_worldTransform = m_localTransform * m_pParentNode->m_worldTransform;
	}
	else
	{
		m_worldTransform = m_localTransform;
	}
}

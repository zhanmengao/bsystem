// Code generated by protoc-gen-gogo. DO NOT EDIT.
// source: table_server.proto

package main

import (
	fmt "fmt"
	proto "github.com/gogo/protobuf/proto"
	io "io"
	math "math"
	math_bits "math/bits"
)

// Reference imports to suppress errors if they are not otherwise used.
var _ = proto.Marshal
var _ = fmt.Errorf
var _ = math.Inf

// This is a compile-time assertion to ensure that this generated file
// is compatible with the proto package it is being compiled against.
// A compilation error at this line likely means your copy of the
// proto package needs to be updated.
const _ = proto.GoGoProtoPackageIsVersion3 // please upgrade the proto package

// 网关
// @cfg:ServerGateConfig|map:ServerGateSystemEventDeliveryConfig|SystemEventType:int32 # 网关事件转发
type ServerGateSystemEventDeliveryConfigAry struct {
	Ary                  []*ServerGateSystemEventDeliveryConfig `protobuf:"bytes,1,rep,name=Ary,proto3" json:"Ary,omitempty"`
	XXX_NoUnkeyedLiteral struct{}                               `json:"-"`
	XXX_unrecognized     []byte                                 `json:"-"`
	XXX_sizecache        int32                                  `json:"-"`
}

func (m *ServerGateSystemEventDeliveryConfigAry) Reset() {
	*m = ServerGateSystemEventDeliveryConfigAry{}
}
func (m *ServerGateSystemEventDeliveryConfigAry) String() string { return proto.CompactTextString(m) }
func (*ServerGateSystemEventDeliveryConfigAry) ProtoMessage()    {}
func (*ServerGateSystemEventDeliveryConfigAry) Descriptor() ([]byte, []int) {
	return fileDescriptor_4452d64c0fb59e92, []int{0}
}
func (m *ServerGateSystemEventDeliveryConfigAry) XXX_Unmarshal(b []byte) error {
	return m.Unmarshal(b)
}
func (m *ServerGateSystemEventDeliveryConfigAry) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	if deterministic {
		return xxx_messageInfo_ServerGateSystemEventDeliveryConfigAry.Marshal(b, m, deterministic)
	} else {
		b = b[:cap(b)]
		n, err := m.MarshalToSizedBuffer(b)
		if err != nil {
			return nil, err
		}
		return b[:n], nil
	}
}
func (m *ServerGateSystemEventDeliveryConfigAry) XXX_Merge(src proto.Message) {
	xxx_messageInfo_ServerGateSystemEventDeliveryConfigAry.Merge(m, src)
}
func (m *ServerGateSystemEventDeliveryConfigAry) XXX_Size() int {
	return m.Size()
}
func (m *ServerGateSystemEventDeliveryConfigAry) XXX_DiscardUnknown() {
	xxx_messageInfo_ServerGateSystemEventDeliveryConfigAry.DiscardUnknown(m)
}

var xxx_messageInfo_ServerGateSystemEventDeliveryConfigAry proto.InternalMessageInfo

func (m *ServerGateSystemEventDeliveryConfigAry) GetAry() []*ServerGateSystemEventDeliveryConfig {
	if m != nil {
		return m.Ary
	}
	return nil
}

type ServerGateSystemEventDeliveryConfig struct {
	SystemEventType      int32    `protobuf:"varint,1,opt,name=SystemEventType,proto3" json:"SystemEventType,omitempty"`
	ToServers            []string `protobuf:"bytes,2,rep,name=ToServers,proto3" json:"ToServers,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}

func (m *ServerGateSystemEventDeliveryConfig) Reset()         { *m = ServerGateSystemEventDeliveryConfig{} }
func (m *ServerGateSystemEventDeliveryConfig) String() string { return proto.CompactTextString(m) }
func (*ServerGateSystemEventDeliveryConfig) ProtoMessage()    {}
func (*ServerGateSystemEventDeliveryConfig) Descriptor() ([]byte, []int) {
	return fileDescriptor_4452d64c0fb59e92, []int{1}
}
func (m *ServerGateSystemEventDeliveryConfig) XXX_Unmarshal(b []byte) error {
	return m.Unmarshal(b)
}
func (m *ServerGateSystemEventDeliveryConfig) XXX_Marshal(b []byte, deterministic bool) ([]byte, error) {
	if deterministic {
		return xxx_messageInfo_ServerGateSystemEventDeliveryConfig.Marshal(b, m, deterministic)
	} else {
		b = b[:cap(b)]
		n, err := m.MarshalToSizedBuffer(b)
		if err != nil {
			return nil, err
		}
		return b[:n], nil
	}
}
func (m *ServerGateSystemEventDeliveryConfig) XXX_Merge(src proto.Message) {
	xxx_messageInfo_ServerGateSystemEventDeliveryConfig.Merge(m, src)
}
func (m *ServerGateSystemEventDeliveryConfig) XXX_Size() int {
	return m.Size()
}
func (m *ServerGateSystemEventDeliveryConfig) XXX_DiscardUnknown() {
	xxx_messageInfo_ServerGateSystemEventDeliveryConfig.DiscardUnknown(m)
}

var xxx_messageInfo_ServerGateSystemEventDeliveryConfig proto.InternalMessageInfo

func (m *ServerGateSystemEventDeliveryConfig) GetSystemEventType() int32 {
	if m != nil {
		return m.SystemEventType
	}
	return 0
}

func (m *ServerGateSystemEventDeliveryConfig) GetToServers() []string {
	if m != nil {
		return m.ToServers
	}
	return nil
}

func init() {
	proto.RegisterType((*ServerGateSystemEventDeliveryConfigAry)(nil), "pb.ServerGateSystemEventDeliveryConfigAry")
	proto.RegisterType((*ServerGateSystemEventDeliveryConfig)(nil), "pb.ServerGateSystemEventDeliveryConfig")
}

func init() { proto.RegisterFile("table_server.proto", fileDescriptor_4452d64c0fb59e92) }

var fileDescriptor_4452d64c0fb59e92 = []byte{
	// 187 bytes of a gzipped FileDescriptorProto
	0x1f, 0x8b, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xff, 0xe2, 0x12, 0x2a, 0x49, 0x4c, 0xca,
	0x49, 0x8d, 0x2f, 0x4e, 0x2d, 0x2a, 0x4b, 0x2d, 0xd2, 0x2b, 0x28, 0xca, 0x2f, 0xc9, 0x17, 0x62,
	0x2a, 0x48, 0x52, 0x4a, 0xe6, 0x52, 0x0b, 0x06, 0x8b, 0xb9, 0x27, 0x96, 0xa4, 0x06, 0x57, 0x16,
	0x97, 0xa4, 0xe6, 0xba, 0x96, 0xa5, 0xe6, 0x95, 0xb8, 0xa4, 0xe6, 0x64, 0x96, 0xa5, 0x16, 0x55,
	0x3a, 0xe7, 0xe7, 0xa5, 0x65, 0xa6, 0x3b, 0x16, 0x55, 0x0a, 0x59, 0x72, 0x31, 0x3b, 0x16, 0x55,
	0x4a, 0x30, 0x2a, 0x30, 0x6b, 0x70, 0x1b, 0xa9, 0xeb, 0x15, 0x24, 0xe9, 0x11, 0xa1, 0x31, 0x08,
	0xa4, 0x47, 0x29, 0x97, 0x4b, 0x99, 0x08, 0xb5, 0x42, 0x1a, 0x5c, 0xfc, 0x48, 0x92, 0x21, 0x95,
	0x05, 0xa9, 0x12, 0x8c, 0x0a, 0x8c, 0x1a, 0xac, 0x41, 0xe8, 0xc2, 0x42, 0x32, 0x5c, 0x9c, 0x21,
	0xf9, 0x10, 0x23, 0x8b, 0x25, 0x98, 0x14, 0x98, 0x35, 0x38, 0x83, 0x10, 0x02, 0x4e, 0x12, 0x17,
	0x1e, 0xca, 0x31, 0x9c, 0x78, 0x24, 0xc7, 0x78, 0xe1, 0x91, 0x1c, 0xe3, 0x83, 0x47, 0x72, 0x8c,
	0x2f, 0x1e, 0xc9, 0x31, 0x7c, 0x78, 0x24, 0xc7, 0x98, 0xc4, 0x06, 0xf6, 0xb8, 0x31, 0x20, 0x00,
	0x00, 0xff, 0xff, 0xb8, 0xcc, 0x3b, 0x4c, 0x0e, 0x01, 0x00, 0x00,
}

func (m *ServerGateSystemEventDeliveryConfigAry) Marshal() (dAtA []byte, err error) {
	size := m.Size()
	dAtA = make([]byte, size)
	n, err := m.MarshalToSizedBuffer(dAtA[:size])
	if err != nil {
		return nil, err
	}
	return dAtA[:n], nil
}

func (m *ServerGateSystemEventDeliveryConfigAry) MarshalTo(dAtA []byte) (int, error) {
	size := m.Size()
	return m.MarshalToSizedBuffer(dAtA[:size])
}

func (m *ServerGateSystemEventDeliveryConfigAry) MarshalToSizedBuffer(dAtA []byte) (int, error) {
	i := len(dAtA)
	_ = i
	var l int
	_ = l
	if m.XXX_unrecognized != nil {
		i -= len(m.XXX_unrecognized)
		copy(dAtA[i:], m.XXX_unrecognized)
	}
	if len(m.Ary) > 0 {
		for iNdEx := len(m.Ary) - 1; iNdEx >= 0; iNdEx-- {
			{
				size, err := m.Ary[iNdEx].MarshalToSizedBuffer(dAtA[:i])
				if err != nil {
					return 0, err
				}
				i -= size
				i = encodeVarintTableServer(dAtA, i, uint64(size))
			}
			i--
			dAtA[i] = 0xa
		}
	}
	return len(dAtA) - i, nil
}

func (m *ServerGateSystemEventDeliveryConfig) Marshal() (dAtA []byte, err error) {
	size := m.Size()
	dAtA = make([]byte, size)
	n, err := m.MarshalToSizedBuffer(dAtA[:size])
	if err != nil {
		return nil, err
	}
	return dAtA[:n], nil
}

func (m *ServerGateSystemEventDeliveryConfig) MarshalTo(dAtA []byte) (int, error) {
	size := m.Size()
	return m.MarshalToSizedBuffer(dAtA[:size])
}

func (m *ServerGateSystemEventDeliveryConfig) MarshalToSizedBuffer(dAtA []byte) (int, error) {
	i := len(dAtA)
	_ = i
	var l int
	_ = l
	if m.XXX_unrecognized != nil {
		i -= len(m.XXX_unrecognized)
		copy(dAtA[i:], m.XXX_unrecognized)
	}
	if len(m.ToServers) > 0 {
		for iNdEx := len(m.ToServers) - 1; iNdEx >= 0; iNdEx-- {
			i -= len(m.ToServers[iNdEx])
			copy(dAtA[i:], m.ToServers[iNdEx])
			i = encodeVarintTableServer(dAtA, i, uint64(len(m.ToServers[iNdEx])))
			i--
			dAtA[i] = 0x12
		}
	}
	if m.SystemEventType != 0 {
		i = encodeVarintTableServer(dAtA, i, uint64(m.SystemEventType))
		i--
		dAtA[i] = 0x8
	}
	return len(dAtA) - i, nil
}

func encodeVarintTableServer(dAtA []byte, offset int, v uint64) int {
	offset -= sovTableServer(v)
	base := offset
	for v >= 1<<7 {
		dAtA[offset] = uint8(v&0x7f | 0x80)
		v >>= 7
		offset++
	}
	dAtA[offset] = uint8(v)
	return base
}
func (m *ServerGateSystemEventDeliveryConfigAry) Size() (n int) {
	if m == nil {
		return 0
	}
	var l int
	_ = l
	if len(m.Ary) > 0 {
		for _, e := range m.Ary {
			l = e.Size()
			n += 1 + l + sovTableServer(uint64(l))
		}
	}
	if m.XXX_unrecognized != nil {
		n += len(m.XXX_unrecognized)
	}
	return n
}

func (m *ServerGateSystemEventDeliveryConfig) Size() (n int) {
	if m == nil {
		return 0
	}
	var l int
	_ = l
	if m.SystemEventType != 0 {
		n += 1 + sovTableServer(uint64(m.SystemEventType))
	}
	if len(m.ToServers) > 0 {
		for _, s := range m.ToServers {
			l = len(s)
			n += 1 + l + sovTableServer(uint64(l))
		}
	}
	if m.XXX_unrecognized != nil {
		n += len(m.XXX_unrecognized)
	}
	return n
}

func sovTableServer(x uint64) (n int) {
	return (math_bits.Len64(x|1) + 6) / 7
}
func sozTableServer(x uint64) (n int) {
	return sovTableServer(uint64((x << 1) ^ uint64((int64(x) >> 63))))
}
func (m *ServerGateSystemEventDeliveryConfigAry) Unmarshal(dAtA []byte) error {
	l := len(dAtA)
	iNdEx := 0
	for iNdEx < l {
		preIndex := iNdEx
		var wire uint64
		for shift := uint(0); ; shift += 7 {
			if shift >= 64 {
				return ErrIntOverflowTableServer
			}
			if iNdEx >= l {
				return io.ErrUnexpectedEOF
			}
			b := dAtA[iNdEx]
			iNdEx++
			wire |= uint64(b&0x7F) << shift
			if b < 0x80 {
				break
			}
		}
		fieldNum := int32(wire >> 3)
		wireType := int(wire & 0x7)
		if wireType == 4 {
			return fmt.Errorf("proto: ServerGateSystemEventDeliveryConfigAry: wiretype end group for non-group")
		}
		if fieldNum <= 0 {
			return fmt.Errorf("proto: ServerGateSystemEventDeliveryConfigAry: illegal tag %d (wire type %d)", fieldNum, wire)
		}
		switch fieldNum {
		case 1:
			if wireType != 2 {
				return fmt.Errorf("proto: wrong wireType = %d for field Ary", wireType)
			}
			var msglen int
			for shift := uint(0); ; shift += 7 {
				if shift >= 64 {
					return ErrIntOverflowTableServer
				}
				if iNdEx >= l {
					return io.ErrUnexpectedEOF
				}
				b := dAtA[iNdEx]
				iNdEx++
				msglen |= int(b&0x7F) << shift
				if b < 0x80 {
					break
				}
			}
			if msglen < 0 {
				return ErrInvalidLengthTableServer
			}
			postIndex := iNdEx + msglen
			if postIndex < 0 {
				return ErrInvalidLengthTableServer
			}
			if postIndex > l {
				return io.ErrUnexpectedEOF
			}
			m.Ary = append(m.Ary, &ServerGateSystemEventDeliveryConfig{})
			if err := m.Ary[len(m.Ary)-1].Unmarshal(dAtA[iNdEx:postIndex]); err != nil {
				return err
			}
			iNdEx = postIndex
		default:
			iNdEx = preIndex
			skippy, err := skipTableServer(dAtA[iNdEx:])
			if err != nil {
				return err
			}
			if (skippy < 0) || (iNdEx+skippy) < 0 {
				return ErrInvalidLengthTableServer
			}
			if (iNdEx + skippy) > l {
				return io.ErrUnexpectedEOF
			}
			m.XXX_unrecognized = append(m.XXX_unrecognized, dAtA[iNdEx:iNdEx+skippy]...)
			iNdEx += skippy
		}
	}

	if iNdEx > l {
		return io.ErrUnexpectedEOF
	}
	return nil
}
func (m *ServerGateSystemEventDeliveryConfig) Unmarshal(dAtA []byte) error {
	l := len(dAtA)
	iNdEx := 0
	for iNdEx < l {
		preIndex := iNdEx
		var wire uint64
		for shift := uint(0); ; shift += 7 {
			if shift >= 64 {
				return ErrIntOverflowTableServer
			}
			if iNdEx >= l {
				return io.ErrUnexpectedEOF
			}
			b := dAtA[iNdEx]
			iNdEx++
			wire |= uint64(b&0x7F) << shift
			if b < 0x80 {
				break
			}
		}
		fieldNum := int32(wire >> 3)
		wireType := int(wire & 0x7)
		if wireType == 4 {
			return fmt.Errorf("proto: ServerGateSystemEventDeliveryConfig: wiretype end group for non-group")
		}
		if fieldNum <= 0 {
			return fmt.Errorf("proto: ServerGateSystemEventDeliveryConfig: illegal tag %d (wire type %d)", fieldNum, wire)
		}
		switch fieldNum {
		case 1:
			if wireType != 0 {
				return fmt.Errorf("proto: wrong wireType = %d for field SystemEventType", wireType)
			}
			m.SystemEventType = 0
			for shift := uint(0); ; shift += 7 {
				if shift >= 64 {
					return ErrIntOverflowTableServer
				}
				if iNdEx >= l {
					return io.ErrUnexpectedEOF
				}
				b := dAtA[iNdEx]
				iNdEx++
				m.SystemEventType |= int32(b&0x7F) << shift
				if b < 0x80 {
					break
				}
			}
		case 2:
			if wireType != 2 {
				return fmt.Errorf("proto: wrong wireType = %d for field ToServers", wireType)
			}
			var stringLen uint64
			for shift := uint(0); ; shift += 7 {
				if shift >= 64 {
					return ErrIntOverflowTableServer
				}
				if iNdEx >= l {
					return io.ErrUnexpectedEOF
				}
				b := dAtA[iNdEx]
				iNdEx++
				stringLen |= uint64(b&0x7F) << shift
				if b < 0x80 {
					break
				}
			}
			intStringLen := int(stringLen)
			if intStringLen < 0 {
				return ErrInvalidLengthTableServer
			}
			postIndex := iNdEx + intStringLen
			if postIndex < 0 {
				return ErrInvalidLengthTableServer
			}
			if postIndex > l {
				return io.ErrUnexpectedEOF
			}
			m.ToServers = append(m.ToServers, string(dAtA[iNdEx:postIndex]))
			iNdEx = postIndex
		default:
			iNdEx = preIndex
			skippy, err := skipTableServer(dAtA[iNdEx:])
			if err != nil {
				return err
			}
			if (skippy < 0) || (iNdEx+skippy) < 0 {
				return ErrInvalidLengthTableServer
			}
			if (iNdEx + skippy) > l {
				return io.ErrUnexpectedEOF
			}
			m.XXX_unrecognized = append(m.XXX_unrecognized, dAtA[iNdEx:iNdEx+skippy]...)
			iNdEx += skippy
		}
	}

	if iNdEx > l {
		return io.ErrUnexpectedEOF
	}
	return nil
}
func skipTableServer(dAtA []byte) (n int, err error) {
	l := len(dAtA)
	iNdEx := 0
	depth := 0
	for iNdEx < l {
		var wire uint64
		for shift := uint(0); ; shift += 7 {
			if shift >= 64 {
				return 0, ErrIntOverflowTableServer
			}
			if iNdEx >= l {
				return 0, io.ErrUnexpectedEOF
			}
			b := dAtA[iNdEx]
			iNdEx++
			wire |= (uint64(b) & 0x7F) << shift
			if b < 0x80 {
				break
			}
		}
		wireType := int(wire & 0x7)
		switch wireType {
		case 0:
			for shift := uint(0); ; shift += 7 {
				if shift >= 64 {
					return 0, ErrIntOverflowTableServer
				}
				if iNdEx >= l {
					return 0, io.ErrUnexpectedEOF
				}
				iNdEx++
				if dAtA[iNdEx-1] < 0x80 {
					break
				}
			}
		case 1:
			iNdEx += 8
		case 2:
			var length int
			for shift := uint(0); ; shift += 7 {
				if shift >= 64 {
					return 0, ErrIntOverflowTableServer
				}
				if iNdEx >= l {
					return 0, io.ErrUnexpectedEOF
				}
				b := dAtA[iNdEx]
				iNdEx++
				length |= (int(b) & 0x7F) << shift
				if b < 0x80 {
					break
				}
			}
			if length < 0 {
				return 0, ErrInvalidLengthTableServer
			}
			iNdEx += length
		case 3:
			depth++
		case 4:
			if depth == 0 {
				return 0, ErrUnexpectedEndOfGroupTableServer
			}
			depth--
		case 5:
			iNdEx += 4
		default:
			return 0, fmt.Errorf("proto: illegal wireType %d", wireType)
		}
		if iNdEx < 0 {
			return 0, ErrInvalidLengthTableServer
		}
		if depth == 0 {
			return iNdEx, nil
		}
	}
	return 0, io.ErrUnexpectedEOF
}

var (
	ErrInvalidLengthTableServer        = fmt.Errorf("proto: negative length found during unmarshaling")
	ErrIntOverflowTableServer          = fmt.Errorf("proto: integer overflow")
	ErrUnexpectedEndOfGroupTableServer = fmt.Errorf("proto: unexpected end of group")
)

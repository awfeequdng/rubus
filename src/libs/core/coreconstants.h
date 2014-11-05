/***************************************************************************
 *   This file is part of the Rubus project                                *
 *   Copyright (C) 2012-2014 by Ivan Volkov                                *
 *   wulff007@gmail.com                                                    *
 *                                                                         *
 **                   GNU General Public License Usage                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 **                  GNU Lesser General Public License                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library.                                      *
 *   If not, see <http://www.gnu.org/licenses/>.                           *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/
#ifndef CORECONSTANTS_H
#define CORECONSTANTS_H


namespace Core {
namespace Constants {

//menus
const char M_FUNCTION[]                = "Menu.Function";
const char M_CATALOG[]                 = "Menu.Catalog";
const char M_HELP[]                    = "Menu.Help";

//actions
const char A_CLOSESESSION[]            = "CloseSession";
const char A_CHANGEPASSWORD[]          = "ChangePassword";
const char A_CLOSE[]                   = "Close";

//main widgets
const char W_WELCOME[]                 = "Widget.Welcome";

//security groups
const char S_DELIMITER                 = '.';

const char S_CORE[]                    = "Core";
const char S_CORE_USERS[]              = "Core.Users";
const char S_CORE_REPORTS[]            = "Core.Reports";
const char S_CORE_USER_CAN_CHANGE_PWD[]= "Core.UserCanChangePwd";


} // namespace Constants
} // namespace Core

#endif // CORECONSTANTS_H
